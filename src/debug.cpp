#include "debug.h"
#include <iomanip>
#include <iostream>
#include <string>

void debug_newline() {
    std::cout << std::endl;
}

void debug_info(std::string str) {
    std::cout << "[ " << BOLDCYAN << "DEBUG" << RESET << " ] " << str
              << std::endl;
}

void debug_warn(std::string str) {
    std::cout << "[ " << BOLDYELLOW << "WARN " << RESET << " ] " << str
              << std::endl;
}

void debug_error(std::string str) {
    std::cout << "[ " << BOLDRED << "ERROR" << RESET << " ] " << str
              << std::endl;
}

void debug_str_map(const char* header,
                   std::map<std::string, std::string> stringMap) {
    debug_info((std::string("[") + std::string(BOLDWHITE) +
                std::string(header) + std::string(RESET) + std::string("]"))
                   .c_str());
    for (const auto& val : stringMap) {
        std::cout << "            " << std::left << std::setfill('.')
                  << std::setw(30) << val.first << " -> " << val.second
                  << std::endl;
    }
    std::cout << "          [/" << BOLDWHITE << header << RESET << "]"
              << std::endl;
}