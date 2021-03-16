#ifndef siweb_httpParser_h
#define siweb_httpParser_h
#include <sstream>
#include <string>
#include <map>
#include "http.h"
#include "utils.h"

namespace siweb::http {
class http_parser {
	using header_map = std::map<std::string, std::string>;
	
   public:
    void parse(const std::string& input);
    inline const header_map& get_headers() const { return headers; }
    inline std::string get_body() const { return body.str(); }
    inline httpMethod get_method() const { return method; }
    inline std::string get_uri() const { return uri; }
    inline bool is_message_complete() const { return state_complete; }

   private:
    std::ostringstream curr;
	std::ostringstream body;
	
	header_map headers;
    httpMethod method;
    std::string uri;
	
	bool state_body = false;
	bool state_method_uri = false;
	bool state_complete = false;

    void parse_header(const std::string& line);
	void parse_body(const std::string& line);
    void parse_method_uri(const std::string& line);
	void parse_method(const char* str);
	void parse_uri(const char* str);
    void parse_line(const std::string& line);
	void parse_header_impl(const char* str);
	int get_content_length();
};
}  // namespace siweb::http

#endif