#include "test_controller.h"

#include "base64.h"

using namespace siweb::http;

test_controller::test_controller() {
    CONTROLLER("/test_controller");
    REGISTER(test_controller, GET, "/test", get);
    REGISTER(test_controller, POST, "/post", post);
}

result test_controller::get(const request& req) {
    return "Yay";
}

result test_controller::post(const request& req) {
    return result("Posted");
}