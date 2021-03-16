#ifndef siweb_response_h
#define siweb_response_h

#include <algorithm>
#include <sstream>
#include <vector>
#include "http.h"

namespace siweb::http {
class response {
   public:
    response(siweb::http::httpStatusCode status_code)
        : status_code(status_code) {
        set_content_type("text/plain");
        set_header("Content-Length", "0");
    }

    response(const char* chars,
             int len,
             siweb::http::httpStatusCode status_code)
        : response(status_code) {
        for (int i = 0; i < len; i++) {
            this->chars.push_back(chars[i]);
        }
        set_header("Content-Length", std::to_string(this->chars.size()));
    }

    response(std::vector<char>& chars, siweb::http::httpStatusCode status_code)
        : response(status_code) {
        this->chars = chars;
        set_header("Content-Length", std::to_string(this->chars.size()));
    }

    std::string get_string() const {
        std::ostringstream oss;
        for (auto& ch : chars) {
            oss << ch;
        }
        return oss.str();
    }

    void set_content_type(const char* content_type) {
        this->content_type = std::string(content_type);
        set_header("Content-Type", this->content_type);
    }

    std::string get_content_type() const { return content_type; }

    siweb::http::httpStatusCode get_status_code() const { return status_code; }

    void set_header(std::string key, std::string value) {
        headers[key] = value;
    }

    void unset_header(std::string key) {
        headers.erase(key);
    }

    const std::map<std::string, std::string>& get_headers() const { return headers; }

   private:
    std::vector<char> chars;
    siweb::http::httpStatusCode status_code;
    std::string content_type;
    std::map<std::string, std::string> headers{{"Server", "SiWeb/0.0.1 (Unix)"},
                                    {"Connection", "Closed"}};
};
}  // namespace siweb::http

#endif