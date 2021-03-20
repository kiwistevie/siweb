#ifndef siweb_server_h
#define siweb_server_h

#include "context.h"
#include "parsing/http_parser.h"
#include "routing/router.h"

namespace siweb::http {
class siweb_server {
   public:
    siweb_server(const routing::router& r) : rtr{r} {}
    int start(int argc, char* argv[]);
    bool is_forking() const { return forking; }
    void set_forking(bool forking) { this->forking = forking; }
    int get_fork_limit() const { return fork_limit; }
    void set_fork_limit(int fork_limit) { this->fork_limit = fork_limit; }

   private:
    const std::string HTTP_VERSION{"1.0"};
    const routing::router& rtr;
    void server_process(int fd, context ctx);
    const std::string create_header(const response& response);
    bool send_response(int fd,
                       const std::string& headers,
                       const std::string& content);
    void log_request(const request& request, const response& response);

    bool forking{false};
    int fork_limit{16};
    parsing::http_parser parser;
};
}  // namespace siweb::http

#endif
