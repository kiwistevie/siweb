#ifndef siweb_router_h
#define siweb_router_h

#include "controller.h"
#include "middleware.h"
#include "request.h"
#include "response.h"
#include "route.h"
#include "types.h"

namespace siweb::http {
class router {
   public:
    response_t route(const request& req) const;
    void add_route(siweb::http::route& r) { routes.emplace_back(&r); }

    void register_controller(const controller& contr) {
        for (auto& rt : contr.get_routes()) {
            this->routes.emplace_back(&(*rt));
        }
    }

    void register_middleware(middlewares::middleware& middlew) {
        this->middlewares.push_back(&middlew);
    }

   private:
    std::vector<siweb::http::route*> routes;
    std::vector<siweb::http::middlewares::middleware*> middlewares;
};
}  // namespace siweb::http

#endif