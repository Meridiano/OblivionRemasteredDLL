#pragma once

#include <RE/Oblivion.h>
#include <OBSE/OBSE.h>

// custom
#include <windows.h>
#include <iostream>
#undef min
#undef max
#include "../lib/mini/ini.h"

namespace fs = std::filesystem;

#define LAUNCH_REQUIRED "Launch required, reason = {}:{}"
#define CRITICAL(FMT, ...) OBSE::stl::report_and_fail(std::format(FMT, __VA_ARGS__))
#define RELATIVE(P, V) (Utility::WorkingDirectory / fs::path(P)).lexically_normal(); REX::INFO("Path."#V" = {}", V.string())
