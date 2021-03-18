#ifndef siweb_route_h
#define siweb_route_h

#include "request.h"
#include "response.h"
#include "result.h"
#include "types.h"

namespace siweb::http {
class route {
   public:
    virtual bool is_hit(const request& req) const = 0;
    virtual result operator()(const request& req) const = 0;
};
}  // namespace siweb::http

#endif