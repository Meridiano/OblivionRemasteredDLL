#pragma once

#include <RE/Oblivion.h>
#include <OBSE/OBSE.h>

// custom
#include <windows.h>
#include <iostream>
#undef min
#undef max
#pragma comment(lib, "crypt32.lib")
#include "../lib/mini/ini.h"

namespace fs = std::filesystem;

#define CRITICAL(FMT, ...) OBSE::stl::report_and_fail(std::format(FMT, __VA_ARGS__))
