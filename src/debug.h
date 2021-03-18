#ifndef siweb_debug_h
#define siweb_debug_h

#include <map>
#include <string>
#include "color.h"

#ifdef DEBUG
#define DEBUG_INFO(str) debug_info((str))
#define DEBUG_ERROR(str) debug_error((str))
#define DEBUG_STR_MAP(header, strmap) debug_str_map((header), (strmap))
#define DEBUG_NEWLINE() debug_newline()
#else
#define DEBUG_INFO(str)
#define DEBUG_ERROR(str)
#define DEBUG_STR_MAP(header, strmap)
#define DEBUG_NEWLINE()
#endif

void debug_newline();
void debug_info(std::string str);
void debug_error(const char* str);
void debug_str_map(const char* header,
                   std::map<std::string, std::string> stringMap);

#endif