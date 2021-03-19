#ifndef siweb_basic_auth_middleware_h
#define siweb_basic_auth_middleware_h

#include <optional>
#include <string>
#include "../request.h"
#include "../types.h"
#include "middleware.h"

namespace siweb::http::middlewares {
class basic_auth_middleware : public middleware {
   public:
    bool accept(const request& req, response_t resp) override;
    std::optional<std::string> get_password(std::string username) const;
    void add_credential(std::string username, std::string password) {
        m_credentials[username] = password;
    }

   private:
    string_map m_credentials;
};
}  // namespace siweb::http::middlewares

#endif