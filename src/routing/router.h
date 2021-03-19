#ifndef siweb_router_h
#define siweb_router_h

#include "../controller.h"
#include "../middlewares/middleware.h"
#include "../request.h"
#include "../response.h"
#include "../types.h"
#include "route.h"

namespace siweb::http::routing {
class router {
   public:
    response_t route(const request& req) const;
    void add_route(routing::route& r) { routes.emplace_back(&r); }

    void register_controller(const controller& contr) {
        for (auto& rt : contr.get_routes()) {
            this->routes.emplace_back(&(*rt));
        }
    }

    void register_middleware(middlewares::middleware& middlew) {
        this->middlewares.push_back(&middlew);
    }

   private:
    std::vector<routing::route*> routes;
    std::vector<middlewares::middleware*> middlewares;
};
}  // namespace siweb::http::routing

#endif