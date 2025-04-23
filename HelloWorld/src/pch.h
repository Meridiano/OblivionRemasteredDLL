#pragma once

#include "OBSE/OBSE.h"
#include "RE/Oblivion.h"

using namespace std::literals;

#define OBRE_DLL(REASON) extern "C" int __stdcall DllMain(void* /* module */, unsigned long REASON, void* /* reserved */)
