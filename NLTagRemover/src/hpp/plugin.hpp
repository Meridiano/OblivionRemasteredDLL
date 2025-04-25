#pragma once
#include "hpp/utility.hpp"

namespace Plugin {

    static const std::string pluginName = "NLTagRemover";

    static const auto pluginPath = []() {
        HMODULE hModule = nullptr;
        GetModuleHandleExA(0x2 + 0x4, (LPCSTR)&pluginName, &hModule);
        return fs::path(Utility::GetModulePathSafe(hModule));
    } ();

    static const auto pluginVersion = []() {
        auto version = REL::GetFileVersion(pluginPath.wstring());
        return (version.has_value() ? version.value().string(".") : "Unknown");
    } ();

    static const auto iniPath = []() {
        fs::path result = pluginPath;
        result.replace_extension(".ini");
        return result;
    } ();

    static const auto logPath = []() {
        fs::path result = pluginPath;
        result.replace_extension(".log");
        return result;
    } ();
    
}
