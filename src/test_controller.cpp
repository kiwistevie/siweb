#include "test_controller.h"

using namespace siweb::http;
using namespace std::placeholders;

test_controller::test_controller() {
    register_endpoint(httpMethod::GET, "/test",
                      std::bind(&test_controller::get, this, _1));
}

response test_controller::get(const request& req) {
    response r("Hello", 5, httpStatusCode::Ok);
    return r;
}