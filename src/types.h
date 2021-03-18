#ifndef siweb_types_h
#define siweb_types h

#include <map>
#include <memory>
#include <string>
#include "response.h"

namespace siweb::http {
using string_map = std::map<std::string, std::string>;
using response_t = std::shared_ptr<response>;
}  // namespace siweb::http

#endif