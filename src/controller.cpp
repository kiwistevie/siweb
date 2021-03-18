#include "controller.h"
#include "lambda_route.h"

using namespace siweb::http;

void controller::register_endpoint(httpMethod method,
                                   std::string uri,
                                   std::function<result(const request&)> func) {
    std::string uri_prefix = this->uri_prefix;
    std::shared_ptr<route> r = std::make_shared<lambda_route>(
        [uri_prefix, method, uri](const request& req) -> bool {
            if (req.get_method() == method &&
                req.get_uri() == (uri_prefix + uri)) {
                return true;
            }
            return false;
        },
        [func](const request& req) -> result { return func(req); });
    routes.push_back(r);
}