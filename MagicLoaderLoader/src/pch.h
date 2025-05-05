#pragma once

#include <RE/Oblivion.h>
#include <OBSE/OBSE.h>

// custom
#include <windows.h>
#include "../lib/mini/ini.h"
#include "spdlog/sinks/basic_file_sink.h"
#undef min
#undef max

namespace fs = std::filesystem;

#define LAUNCH_REQUIRED "Launch required, reason = {}:{}"
#define LOG(...) if (Utility::logReady) spdlog::info(std::format(__VA_ARGS__))
#define OB64_DLL(REASON) extern "C" int __stdcall DllMain(void*, REASON, void*)
#define RELATIVE(PATH) (Utility::workingDirectory / fs::path(PATH)).lexically_normal()
#define CRITICAL(INFO) if (std::string err = INFO; true) { LOG("{}", err); throw(std::exception(err.data())); }
