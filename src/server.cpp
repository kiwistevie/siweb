#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <string>

#include "context.h"

#define MAX_FORKS 16

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
        fprintf(stderr, "Error while waiting for child\n");
    } else {
        install_childterm_signal();
    }
    num_forks--;
}

int server_start(int argc, char* argv[]) {
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
            fprintf(stderr, "Setsockopt() failed\n");
            close(sfd);
            exit(1);
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break; /* Success */

        close(sfd);
    }

    if (rp == NULL) { /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result); /* No longer needed */

    if ((listen(sfd, 5)) != 0) {
        fprintf(stderr, "Listen failed ... \n");
        exit(0);
    } else {
        printf("Listening on port %s ... \n", argv[0]);
    }

    install_childterm_signal();

    while (1) {
        if (num_forks > MAX_FORKS)
            continue;
        peer_addr_len = sizeof(struct sockaddr_storage);
        int cfd = accept(sfd, (struct sockaddr*)&peer_addr, &peer_addr_len);

        num_forks++;
        int p_id = fork();

        if (p_id < 0) {
            fprintf(stderr, "Forking failed\n");
            close(cfd);
        } else if (p_id == 0) {
            if (cfd == -1) {
                fprintf(stderr, "Accept error\n");
                exit(1);
            } else {
                close(sfd);
                char hostbuf[255];
                context ctx{get_ip_str((struct sockaddr*)&peer_addr, hostbuf,
                                       peer_addr_len),
                            (int)get_host_str((struct sockaddr*)&peer_addr)};
                server_process(cfd, std::move(ctx));
                close(cfd);
                exit(0);
            }
        } else {
            close(cfd);
        }
    }

    close(sfd);
}
