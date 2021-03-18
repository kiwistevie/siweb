#ifndef siweb_router_h
#define siweb_router_h

#include "controller.h"
#include "request.h"
#include "response.h"
#include "route.h"

namespace siweb::http {
class router {
   public:
    response route(const request& req) const;
    void add_route(siweb::http::route& r) {
        routes.emplace_back(std::shared_ptr<siweb::http::route>(&r));
    }

    void register_controller(const controller& contr) {
        for (auto rt : contr.get_routes()) {
            this->routes.push_back(rt);
        }
    }

   private:
    std::vector<std::shared_ptr<siweb::http::route>> routes;
};
}  // namespace siweb::http

#endif