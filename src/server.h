#ifndef siweb_server_h
#define siweb_server_h

#include "context.h"
#include "router.h"

namespace siweb::http {
class siweb_server {
   public:
    siweb_server(const router& r) : rtr{r} {}
    int start(int argc, char* argv[]);
    bool is_forking() const { return forking; }
    void set_forking(bool forking) { this->forking = forking; }
    int get_fork_limit() const { return fork_limit; }
    void set_fork_limit(int fork_limit) { this->fork_limit = fork_limit; }

   private:
    const router& rtr;
    void server_process(int fd, context ctx);

    bool forking{false};
    int fork_limit{16};
};
}  // namespace siweb::http

#endif
