#include "server.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include "color.h"
#include "context.h"
#include "debug.h"
#include "http_parser.h"
#include "log.h"
#include "types.h"

#define MAX_FORKS 16
#define READ_BUFFER_SIZE 1024

using namespace siweb::http;

extern void server_process(int fd, const context& ctx);
static void waiter(int pid);

int num_forks = 0;

static char* get_ip_str(const struct sockaddr* sa, char* s, size_t maxlen) {
    switch (sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in*)sa)->sin_addr), s,
                      maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6*)sa)->sin6_addr), s,
                      maxlen);
            break;

        default:
            strncpy(s, "Unknown family", maxlen);
            break;
    }

    return s;
}

static unsigned short get_host_str(const struct sockaddr* sa) {
    switch (sa->sa_family) {
        case AF_INET:
            return ntohs(((struct sockaddr_in*)sa)->sin_port);
            break;

        case AF_INET6:
            return ntohs(((struct sockaddr_in6*)sa)->sin6_port);
            break;

        default:
            return 0;
    }
}

static void install_childterm_signal() {
    signal(SIGCHLD, waiter);
}

static void waiter(int pid) {
    int child_pid, status_code;
    child_pid = wait(&status_code);
    if (child_pid == -1) {
        DEBUG_ERROR("Error while waiting for child\n");
    } else {
        install_childterm_signal();
    }
    num_forks--;
}

void siweb_server::server_process(int fd, context ctx) {
    DEBUG_INFO(
        "---------------------------- PROCESSING REQUEST "
        "----------------------------");
    int ret;
    char buff[READ_BUFFER_SIZE];

    http_parser parser;
    DEBUG_INFO("Reading data from client ...");
    while (true) {
        if ((ret = recv(fd, buff, sizeof(buff), 0)) > 0) {
            parser.parse(std::string(buff, buff + ret));
            if (parser.is_message_complete()) {
                break;
            }
        } else {
            exit(0);
        }
    }

    DEBUG_INFO("Generating request object ...");
    request req(parser.get_method(), parser.get_uri(), ctx.ip_addr,
                parser.get_headers(), parser.get_parameters());
    req.set_body(parser.get_body());

    DEBUG_INFO("Routing request and generating response ...");
    auto resp = this->rtr.route(req);

    LOG((std::string(BOLDWHITE) + http::HttpMethodToString(req.get_method()) +
         RESET + " " + req.get_uri() + " |> " +
         std::to_string((int)resp->get_status_code()) + " " +
         http::HttpStatusCodeToString(resp->get_status_code()))
            .c_str());

    std::ostringstream oss;
    oss << "HTTP/1.1 " << (int)resp->get_status_code() << " "
        << http::HttpStatusCodeToString(resp->get_status_code()) << std::endl;
    for (auto& header : resp->get_headers()) {
        oss << header.first << ": " << header.second << std::endl;
    }

    oss << std::endl;

    std::string response = oss.str();
    std::string content = resp->get_content();
    DEBUG_INFO("Writing response to client ...");
    write(fd, response.data(), response.length());
    if (content.length() > 0) {
        write(fd, content.data(), content.length());
    }
    close(fd);
    DEBUG_INFO("Connection closed.");
    DEBUG_INFO(
        "-------------------------- END PROCESSING REQUEST "
        "--------------------------");
}

int siweb_server::start(int argc, char* argv[]) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       /* IPv4 */
    hints.ai_socktype = SOCK_STREAM; /* TCP socket */

    DEBUG_INFO("Getting address info ...");
    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully bind(2).
       If socket(2) (or bind(2)) fails, we (close the socket
       and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        int option = 1;
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, (char*)&option,
                       sizeof(option)) < 0) {
            DEBUG_ERROR("Setsockopt() failed");
            close(sfd);
            exit(1);
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            DEBUG_INFO("Bound to socket");
            break; /* Success */
        }

        close(sfd);
    }

    if (rp == NULL) { /* No address succeeded */
        DEBUG_ERROR("Could not bind");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result); /* No longer needed */

    if (forking) {
        DEBUG_WARN("Forking is enabled - no shared memory between requests!");
    }

    if ((listen(sfd, 5)) != 0) {
        DEBUG_ERROR("Listen failed ... ");
        exit(0);
    } else {
        LOG("Listening ...");
    }

    install_childterm_signal();

    while (1) {
        if (forking && num_forks > MAX_FORKS)
            continue;
        peer_addr_len = sizeof(struct sockaddr_storage);
        int cfd = accept(sfd, (struct sockaddr*)&peer_addr, &peer_addr_len);

        int p_id = 0;
        if (forking) {
            num_forks++;
            p_id = fork();
        }

        if (forking && p_id < 0) {
            fprintf(stderr, "Forking failed\n");
            close(cfd);
        } else if (!forking || p_id == 0) {
            if (cfd == -1) {
                fprintf(stderr, "Accept error\n");
                exit(1);
            } else {
                if (forking)
                    close(sfd);
                char hostbuf[255];
                context ctx{get_ip_str((struct sockaddr*)&peer_addr, hostbuf,
                                       peer_addr_len),
                            (int)get_host_str((struct sockaddr*)&peer_addr)};
                server_process(cfd, std::move(ctx));
                close(cfd);
                if (forking)
                    exit(0);
            }
        } else {
            close(cfd);
        }
    }

    close(sfd);
}
