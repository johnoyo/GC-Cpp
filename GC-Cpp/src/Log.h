#pragma once

#include "ProfilerScope.h"

#include <string>
#include <iostream>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifdef DEBUG
#define LOG(f_, ...) printf(("LOG[%s]: " f_ " (%s::%d)\n"), __TIME__, ##__VA_ARGS__, __FILENAME__, __LINE__)
#define PROFILE(...) ProfilerScope profiler = ProfilerScope(__VA_ARGS__);
#define FUNC_PROFILE() PROFILE(__FUNCTION__)
#else
#define LOG(f_, ...)
#define PROFILE(...) ProfilerScope profiler = ProfilerScope(__VA_ARGS__);
#define FUNC_PROFILE() PROFILE(__FUNCTION__)
#endif // _DEBUG
