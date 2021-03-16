#ifndef siweb_file_route_h
#define siweb_file_route_h

#include <fstream>
#include <streambuf>
#include <string>
#include "http.h"
#include "request.h"
#include "response.h"
#include "router.h"
#include "utils.h"

namespace siweb::http {
class file_route : public route {
   public:
    file_route(const char* uri, const char* root_path)
        : uri(uri), root_path(root_path) {}

    bool is_hit(const request& req) const override;
    response operator()(const request& req) const override;

   private:
    const std::string uri;
    const std::string root_path;
};
}  // namespace siweb::http

#endif
