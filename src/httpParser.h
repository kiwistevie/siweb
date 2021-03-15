#ifndef siweb_httpParser_h
#define siweb_httpParser_h
#include <sstream>
#include <string>
#include <vector>
#include "http.h"
#include "utils.h"

class HttpParser {
 public:
  void parse(const std::string& input) {
    if (complete)
      return;

    std::vector<std::string> lines;
    for (const auto& ch : input) {
      if (ch != '\n') {
        curr << ch;
      } else {
        lines.push_back(trim_copy(curr.str()));
        curr.str("");
        curr.clear();
      }
    }

    for (const auto& line : lines) {
      parse_line(line);
    }
  }

  const std::vector<HttpHeader>& get_headers() const { return headers; }

  std::string get_body() const { return body.str(); }

  HttpMethod get_method() const { return method; }

  std::string get_uri() const { return uri; }

  bool is_message_complete() const { return complete; }

 private:
  bool complete = false;
  bool parse_body = false;
  int body_size = 0;
  std::ostringstream body;
  std::vector<HttpHeader> headers;
  std::ostringstream curr;

  bool method_uri_parsed = false;
  HttpMethod method;
  std::string uri;

  void parse_header(const std::string& line) {
    HttpHeader header;
    std::ostringstream identifier;
    std::ostringstream value;
    bool delim = false;
    for (const auto& ch : line) {
      if (ch == ':' && !delim) {
        delim = true;
        continue;
      }

      if (delim) {
        value << ch;
      } else {
        identifier << ch;
      }
    }
    header.value = trim_copy(value.str());
    header.identifier = trim_copy(identifier.str());
    headers.push_back(header);
  }

  void parse_method_uri(const std::string& line) {
    std::ostringstream method;
    std::ostringstream uri;
    bool sw = false;
    for (const auto& ch : line) {
      if (ch == ' ' && !sw) {
        sw = true;
        continue;
      } else if (ch == ' ' && sw) {
        break;
      }

      if (!sw) {
        method << ch;
      } else {
        uri << ch;
      }
    }

    std::string m = method.str();
    if (m == "GET")
      this->method = HttpMethod::GET;
    else if (m == "POST")
      this->method = HttpMethod::POST;

    this->uri = uri.str();
  }

  void parse_line(const std::string& line) {
    if (line.length() == 0) {
      auto it = std::find_if(std::begin(headers), std::end(headers),
                             [](const HttpHeader& header) {
                               return header.identifier == "Content-Length";
                             });
      if (it != std::end(headers)) {
        auto& header = *it;
        body_size = std::atoi(header.value.c_str());
        parse_body = true;
        return;
      } else {
        complete = true;
        return;
      }
    }

    if (!method_uri_parsed) {
      parse_method_uri(line);
      method_uri_parsed = true;
    } else {
      if (parse_body) {
        body << line;
      } else {
        parse_header(line);
      }
    }

    if (parse_body && body.str().length() == body_size)
      complete = true;
  }
};

#endif