#pragma once
#include "_config.h"
#include "_precl.h"
#include <string>


extern std::string GetAsiPath();
extern std::string GetAsiName();
extern bool IsVersion_1_0();
extern bool IsAsiLoaderUsed();
extern bool file_exists(const char *name);
extern std::string ToUpper(const std::string &str);
extern std::string ToLower(const std::string &str);
