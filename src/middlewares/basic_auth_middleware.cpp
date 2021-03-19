#include "basic_auth_middleware.h"
#include <tuple>
#include "../encoding/base64.h"

using namespace siweb::http::middlewares;

static bool try_parse_basic(std::string str, std::string& out) {
    if (str.length() > 6) {
        if (str.substr(0, 6) == "Basic ") {
            out = str.substr(6);
            return true;
        }
    }

    out = "";
    return false;
}

static std::tuple<std::string, std::string> get_username_password(
    std::string str) {
    for (int i = 0; i < str.length() - 1; i++) {
        if (str[i] == ':')
            return {str.substr(0, i), str.substr(i + 1)};
    }
    return {};
}

bool basic_auth_middleware::accept(const request& req, response_t resp) {
    auto auth_header = req.get_header("authorization");
    std::string base64;
    if (auth_header.has_value() &&
        try_parse_basic(auth_header.value(), base64)) {
        auto plain = encoding::base64_decode(base64);
        auto credentials = get_username_password(plain);
        auto password = get_password(std::get<0>(credentials));
        if (password.has_value()) {
            if (password.value() == std::get<1>(credentials)) {
                return true;
            }
        }
    }

    resp->set_header("WWW-Authenticate", "Basic realm=\"Test\"");
    resp->set_status_code(httpStatusCode::Unauthorized);
    return false;
}

std::optional<std::string> basic_auth_middleware::get_password(
    std::string username) const {
    auto it = m_credentials.find(username);
    if (it != std::end(m_credentials)) {
        return it->second;
    }
    return {};
}