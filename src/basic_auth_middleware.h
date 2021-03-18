#ifndef siweb_basic_auth_middleware_h
#define siweb_basic_auth_middleware_h

#include <optional>
#include <string>
#include "middleware.h"
#include "request.h"
#include "types.h"

namespace siweb::http::middlewares {
class basic_auth_middleware : public middleware {
   public:
    bool accept(const request& req, response_t resp) override {
        auto auth_header = req.get_header("authorization");
        if (auth_header.has_value()) {
            return true;
        } else {
            resp->set_header("WWW-Authenticate", "Basic realm=\"Test\"");
            resp->set_status_code(httpStatusCode::Unauthorized);
            return false;
        }
    }

    void add_credential(std::string username, std::string password) {
        m_credentials[username] = password;
    }

    std::optional<std::string> get_password(std::string username) const {
        auto it = m_credentials.find(username);
        if (it != std::end(m_credentials)) {
            return it->second;
        }
        return {};
    }

   private:
    string_map m_credentials;
};
}  // namespace siweb::http::middlewares

#endif