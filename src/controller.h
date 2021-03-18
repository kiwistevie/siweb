#ifndef siweb_controller_h
#define siweb_controller_h

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "http.h"
#include "request.h"
#include "result.h"
#include "route.h"
#include "types.h"

#define CONTROLLER(uri) this->set_uri_prefix((uri))

#define REGISTER(class, method, uri, func)       \
    register_endpoint(httpMethod::method, (uri), \
                      std::bind(&class ::func, this, std::placeholders::_1))

namespace siweb::http {
class controller {
   public:
    using route_ptr_t = std::shared_ptr<route>;
    virtual void register_endpoint(httpMethod method,
                                   std::string uri,
                                   std::function<result(const request&)> func);

    const std::vector<route_ptr_t>& get_routes() const noexcept {
        return routes;
    }

    void set_uri_prefix(std::string uri_prefix) noexcept {
        this->uri_prefix = uri_prefix;
    }

   private:
    std::vector<route_ptr_t> routes;
    std::string uri_prefix;
};
}  // namespace siweb::http

#endif