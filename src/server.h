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

   private:
    const router& rtr;
    void server_process(int fd, context ctx);
    bool forking{false};
};
}  // namespace siweb::http

#endif
