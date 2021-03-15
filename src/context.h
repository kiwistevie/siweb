#ifndef siweb_context_h
#define siweb_context_h

#include <string>

namespace siweb::http {
struct context {
    std::string ip_addr;
    int port;
};
}  // namespace siweb::http

#endif
