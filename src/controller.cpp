#include "controller.h"
#include "lambda_route.h"

using namespace siweb::http;

void controller::register_endpoint(
    httpMethod method,
    std::string uri,
    std::function<response(const request&)> func) {
    std::shared_ptr<route> r = std::make_shared<lambda_route>(
        [method, uri](const request& req) -> bool {
            if (req.get_method() == method && req.get_uri() == uri) {
                return true;
            }
            return false;
        },
        [func](const request& req) -> response { return func(req); });
    routes.push_back(r);
}