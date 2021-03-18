#include "log.h"
#include <iostream>
#include "color.h"

void log(const char* str) {
    std::cout << "[ " << BOLDBLUE << "INFO" << RESET << "  ] " << str
              << std::endl;
}