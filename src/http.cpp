#include "http.h"

using namespace siweb::http;

std::map<std::string, std::string> http::content_type_mappings = {
    {".html", "text/html"},
    {".xml", "application/xml"},
    {".txt", "text/plain"}};
