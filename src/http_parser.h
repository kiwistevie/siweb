#ifndef siweb_httpParser_h
#define siweb_httpParser_h
#include <map>
#include <sstream>
#include <string>
#include "http.h"
#include "utils.h"

namespace siweb::http {
class http_parser {
   public:
    using string_map = std::map<std::string, std::string>;

    void parse(const std::string& input);
    const string_map& get_headers() const { return headers; }
    const string_map& get_parameters() const { return parameters; }
    std::string get_body() const noexcept { return body; }
    constexpr httpMethod get_method() const noexcept { return method; }
    std::string get_uri() const noexcept { return uri; }
    constexpr bool is_message_complete() const noexcept {
        return state_complete;
    }

   private:
    std::string input{""};
    int start{0};
    int current{0};

    string_map headers;
    string_map parameters;
    httpMethod method;
    std::string uri;
    std::string body;

    bool state_body = false;
    bool state_method_uri = true;
    bool state_complete = false;

    void parse();
    void parse_method();
    void parse_uri();
    void parse_header();
    void panic();

    bool isAlpha(char c);
    bool isDigit(char c);
    bool isAtEnd();
    char advance();
    char peek();
    char peekNext();
    bool match(char expected);
    void skipWhitespace();
    std::string makeString();
    int get_content_length();
};
}  // namespace siweb::http

#endif