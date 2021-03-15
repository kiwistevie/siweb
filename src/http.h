#ifndef siweb_http_h
#define siweb_http_h

#include <string>

struct HttpHeader {
  std::string identifier;
  std::string value;
};

enum HttpMethod { GET, POST, UPDATE, PUT, DELETE };

#endif