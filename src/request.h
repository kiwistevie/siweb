#ifndef siweb_request_h
#define siweb_request_h

#include "http.h"

namespace siweb::http {
class request {
   public:
    request(const httpMethod& method,
            const std::string& uri,
            const std::string& client_ip)
        : method{method}, uri{uri}, client_ip{client_ip} {}

    const httpMethod& get_method() const { return method; }
    std::string get_uri() const { return uri; }
    std::string get_client_ip() const { return client_ip; }

   private:
    httpMethod method;
    std::string uri;
    std::string client_ip;
};
}  // namespace siweb::http

#endif