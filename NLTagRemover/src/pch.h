#pragma once

#include "OBSE/OBSE.h"
#include "RE/Oblivion.h"

using namespace std::literals;
namespace fs = std::filesystem;

#include "spdlog/sinks/basic_file_sink.h"
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

#define OB64_DLL(REASON) extern "C" int __stdcall DllMain(void* /* module */, REASON, void* /* reserved */)
#define LOG(FMT, ...) if (Utility::logReady) spdlog::info(std::format(FMT, __VA_ARGS__))
