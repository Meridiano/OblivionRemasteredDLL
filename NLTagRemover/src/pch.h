#pragma once

#include "OBSE/OBSE.h"
#include "RE/Oblivion.h"

using namespace std::literals;

#define OBRE_DLL(REASON) extern "C" int __stdcall DllMain(void* /* module */, unsigned long REASON, void* /* reserved */)
#include "../lib/mini/ini.h"
#include "spdlog/sinks/basic_file_sink.h"
#define LOG(fmt, ...) spdlog::info(std::format(fmt, __VA_ARGS__))
