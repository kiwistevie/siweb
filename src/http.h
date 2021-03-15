#ifndef siweb_http_h
#define siweb_http_h

#include <string>

struct httpHeader {
    std::string identifier;
    std::string value;
};

enum httpMethod { GET, POST, UPDATE, PUT, DELETE };

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
};

#endif