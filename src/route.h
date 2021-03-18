#ifndef siweb_route_h
#define siweb_route_h

#include "request.h"
#include "response.h"

namespace siweb::http {
class route {
   public:
    virtual bool is_hit(const request& req) const = 0;
    virtual response operator()(const request& req) const = 0;
};
}  // namespace siweb::http

#endif