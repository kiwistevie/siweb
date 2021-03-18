#include "test_controller.h"

using namespace siweb::http;

test_controller::test_controller() {
    CONTROLLER("/test_controller");
    REGISTER(test_controller, GET, "/test", get);
    REGISTER(test_controller, POST, "/post", post);
}

response_t test_controller::get(const request& req) {
    std::string content =
        req.get_parameter("message").value_or("No Message provided");
    return std::make_unique<response>(content.c_str(), content.length(),
                                      httpStatusCode::Ok);
}

response_t test_controller::post(const request& req) {
    return std::make_unique<response>("Posted", 6, httpStatusCode::Ok);
}