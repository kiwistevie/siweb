#include "file_route.h"
#include <algorithm>
#include <filesystem>
#include <map>
#include <sstream>
#include <vector>

using namespace siweb::http;
using namespace siweb::http::routing;
using namespace siweb::utils;

namespace fs = std::filesystem;
static std::vector<std::string> get_files_in_directory(std::string path) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(path)) {
        files.push_back(entry.path());
    }
    return files;
}

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
    std::string uri = req.get_uri();
    std::string path = this->root_path + uri.substr(this->uri.length());
    if (path == "")
        path = ".";
    if (path.length() > 0) {
        if (fs::is_regular_file(path)) {
            std::ifstream t(path, std::ios::binary);
            if (t.good()) {
                std::string str((std::istreambuf_iterator<char>(t)),
                                std::istreambuf_iterator<char>());
                auto r = result(str);
                r.set_status_code(httpStatusCode::Ok);
                r.set_content_type(get_content_type(get_file_extension(path)));
                return r;
            }
        } else if (fs::is_directory(path)) {
            auto files = get_files_in_directory(path);
            std::ostringstream oss;
            oss << "<html><head><title>Index of '" << path
                << "'</title><body><h1>" << path << "</h1>"
                << "<ul>";
            for (auto& file : files) {
                oss << "<li><a href=\"" << file << "\">" << file << "</a></li>";
            }
            oss << "</ul></body></html>";
            auto r = result(oss.str());
            r.set_status_code(httpStatusCode::Ok);
            r.set_content_type("text/html");
            return r;
        }
    }

    return result("");
}
