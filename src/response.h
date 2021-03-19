#ifndef siweb_response_h
#define siweb_response_h

#include <algorithm>
#include <sstream>
#include <vector>
#include "http.h"
#include "version.h"

namespace siweb::http {
class response {
   public:
    response() : response("", "text/plain", httpStatusCode::NotFound) {}

    response(siweb::http::httpStatusCode status_code)
        : response("", "text/plain", status_code) {}

    response(std::string content,
             std::string content_type,
             siweb::http::httpStatusCode status_code)
        : content{content},
          content_type{content_type},
          status_code{status_code} {
        update_content_length();
    }

    std::string get_content() const { return content; }

    void set_content(std::string content) {
        this->content = content;
        update_content_length();
    }

    void set_content_type(std::string content_type) {
        this->content_type = std::string(content_type);
        set_header("Content-Type", this->content_type);
    }

    std::string get_content_type() const { return content_type; }

    siweb::http::httpStatusCode get_status_code() const { return status_code; }
    void set_status_code(httpStatusCode status_code) {
        this->status_code = status_code;
    }

    void set_header(std::string key, std::string value) {
        headers[key] = value;
    }

    void unset_header(std::string key) { headers.erase(key); }

    const std::map<std::string, std::string>& get_headers() const {
        return headers;
    }

   private:
    std::string content;
    siweb::http::httpStatusCode status_code;
    std::string content_type;
    std::map<std::string, std::string> headers{{"Server", VERSION},
                                               {"Connection", "close"}};

    void update_content_length() {
        set_header("content-length", std::to_string(content.length()));
    }
};
}  // namespace siweb::http

#endif