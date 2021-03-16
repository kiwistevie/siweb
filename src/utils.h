#ifndef siweb_utils_h
#define siweb_utils_h

#include <algorithm>
#include <sstream>
#include <string>

namespace siweb::utils {
// trim from start (in place)
inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

// trim from both ends (in place)
inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

inline std::string get_file_extension(std::string file) {
    std::ostringstream oss;
    for (int i = file.length() - 1; i >= 0; i--) {
        if (file[i] == '.') {
            oss << file[i];
            break;
        }
        oss << file[i];
    }

    std::string out = oss.str();
    std::reverse(std::begin(out), std::end(out));
    return out;
}
}  // namespace siweb::utils

#endif