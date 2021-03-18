#ifndef siweb_request_h
#define siweb_request_h

//#include <optional>
#include <optional>
#include <vector>
#include "http.h"
#include "types.h"

namespace siweb::http {
class request {
   public:
    request(const httpMethod& method,
            const std::string& uri,
            const std::string& client_ip,
            const string_map& headers,
            const string_map& parameters)
        : m_method{method},
          m_uri{uri},
          m_client_ip{client_ip},
          m_body{},
          m_headers{headers},
          m_parameters{parameters} {}

    void set_body(const std::string& body) { this->m_body = body; }
    const httpMethod& get_method() const { return m_method; }
    const string_map& get_headers() const { return m_headers; }
    const string_map& get_parameters() const { return m_parameters; }
    std::optional<std::string> get_parameter(std::string name) const {
        auto it = m_parameters.find(name);
        if (it != std::end(m_parameters)) {
            return it->second;
        }

        return {};
    }
    std::optional<std::string> get_header(std::string name) const {
        auto it = m_headers.find(name);
        if (it != std::end(m_headers)) {
            return it->second;
        }

        return {};
    }
    std::string get_uri() const { return m_uri; }
    std::string get_client_ip() const { return m_client_ip; }
    std::string get_body() const { return m_body; }

   private:
    httpMethod m_method;
    string_map m_headers;
    string_map m_parameters;
    std::string m_uri;
    std::string m_client_ip;
    std::string m_body;
};
}  // namespace siweb::http

#endif