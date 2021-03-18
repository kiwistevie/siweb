#include "file_route.h"
#include <algorithm>
#include <map>
#include <sstream>

using namespace siweb::http;
using namespace siweb::utils;

bool file_route::is_hit(const request& req) const {
    const auto& uri = req.get_uri();
    if (uri.length() >= this->uri.length()) {
        return uri.substr(0, this->uri.length()) == this->uri;
    }

    return false;
}

const char* get_content_type(std::string extension) {
    if (http::content_type_mappings[extension] != std::string()) {
        return http::content_type_mappings[extension].c_str();
    } else {
        return "text/plain";
    }
}

result file_route::operator()(const request& req) const {
    const auto& uri = req.get_uri();
    std::string path = this->root_path + uri.substr(this->uri.length());
    if (path.length() > 0) {
        std::ifstream t(path, std::ios::binary);
        if (t.good()) {
            std::string str((std::istreambuf_iterator<char>(t)),
                            std::istreambuf_iterator<char>());
            auto r = result(str.c_str());
            r.set_status_code(httpStatusCode::Ok);
            r.set_content_type(get_content_type(get_file_extension(path)));
            return r;
        }
    }

    return result("");
}
