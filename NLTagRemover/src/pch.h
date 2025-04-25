#pragma once

#include "OBSE/OBSE.h"
#include "RE/Oblivion.h"

using namespace std::literals;
namespace fs = std::filesystem;

#define OBRE_DLL(REASON) extern "C" int __stdcall DllMain(void* /* module */, unsigned long REASON, void* /* reserved */)
#include <windows.h>
#include "../lib/mini/ini.h"
#include "spdlog/sinks/basic_file_sink.h"
#define LOG(FMT, ...) spdlog::info(std::format(FMT, __VA_ARGS__))
