#pragma once

#include "OBSE/OBSE.h"
#include "RE/Oblivion.h"

using namespace std::literals;
namespace fs = std::filesystem;

#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#include "../lib/badaix/aixlog.hpp"

#define OB64_DLL(REASON) extern "C" int __stdcall DllMain(void*, REASON, void*)
#define AIX_LOG(...) if (Utility::logReady) LOG(INFO) << std::format(__VA_ARGS__) << "\n"
