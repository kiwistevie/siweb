#ifndef siweb_result_h
#define siweb_result_h

#include <string>
#include "http.h"

namespace siweb::http {
class result {
   public:
    result() {}
    result(const char* content) : content{content} {}
    result(std::string content) : content{content} {}

    void set_status_code(httpStatusCode status_code) {
        this->status_code = status_code;
    }

    httpStatusCode get_status_code() const { return status_code; }

    void set_content_type(std::string content_type) {
        this->content_type = content_type;
    }

    std::string get_content_type() const { return content_type; }

    virtual std::string get_content() const { return content; }

   private:
    std::string content{""};
    httpStatusCode status_code{httpStatusCode::Ok};
    std::string content_type{"text/plain"};
};
}  // namespace siweb::http

#endif