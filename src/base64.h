#ifndef siweb_base64_h
#define siweb_base64_h

#include <string>

namespace siweb::encoding {
std::string base64_encode(unsigned char const* bytes_to_encode,
                          unsigned int in_len);
std::string base64_decode(std::string const& encoded_string);
}  // namespace siweb::encoding
#endif