#ifndef siweb_file_route_h
#define siweb_file_route_h

#include <fstream>
#include <streambuf>
#include <string>
#include "../http.h"
#include "../request.h"
#include "../response.h"
#include "../result.h"
#include "../types.h"
#include "../utils/utils.h"
#include "router.h"

namespace siweb::http::routing {
class file_route : public route {
   public:
    file_route(const char* uri, const char* root_path)
        : uri(uri), root_path(root_path) {}

    bool is_hit(const request& req) const override;
    result operator()(const request& req) const override;

   private:
    const std::string uri;
    const std::string root_path;
};
}  // namespace siweb::http::routing

#endif
