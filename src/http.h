#ifndef siweb_http_h
#define siweb_http_h

#include <string>

namespace siweb::http {
struct httpHeader {
    std::string identifier;
    std::string value;
};

enum httpMethod { GET, POST, UPDATE, PUT, DELETE };
enum httpStatusCode { Ok = 200, NotFound = 404 };

class http {
   public:
    static std::string HttpMethodToString(const httpMethod& method) {
        switch (method) {
            case httpMethod::GET:
                return "GET";
            case httpMethod::POST:
                return "POST";
            case httpMethod::DELETE:
                return "DELETE";
            default:
                return "NOT IMPLEMENTED";
        }
    }

    static std::string HttpStatusCodeToString(
        const httpStatusCode& statusCode) {
        switch (statusCode) {
            case httpStatusCode::Ok:
                return "Ok";
            case httpStatusCode::NotFound:
                return "NotFound";
            default:
                return "NotFound";
        }
    }
};
}  // namespace siweb::http

#endif