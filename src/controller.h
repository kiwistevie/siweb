#ifndef siweb_controller_h
#define siweb_controller_h

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "http.h"
#include "request.h"
#include "response.h"
#include "route.h"

namespace siweb::http {
class controller {
   public:
    using route_ptr_t = std::shared_ptr<route>;

    virtual void register_endpoint(
        httpMethod method,
        std::string uri,
        std::function<response(const request&)> func);

    const std::vector<route_ptr_t>& get_routes() const noexcept {
        return routes;
    }

   private:
    std::vector<route_ptr_t> routes;
};
}  // namespace siweb::http

#endif