#pragma once
#include "hpp/utility.hpp"

namespace Plugin {

    extern "C" OBSE::PluginVersionData OBSEPlugin_Version;

    static const auto pluginName = std::string(OBSEPlugin_Version.pluginName);
    
    static const auto pluginPath = []() {
        HMODULE hModule = nullptr;
        GetModuleHandleExA(0x2 + 0x4, LPCSTR(&pluginName), &hModule);
        return fs::path(Utility::GetModulePathSafe(hModule));
    } ();

    static const auto pluginVersion = []() {
        auto version = REL::GetFileVersion(pluginPath.wstring());
        return (version.has_value() ? version.value().string(".") : "Unknown");
    } ();

    static const auto logPath = []() {
        fs::path result = pluginPath;
        result.replace_extension(".log");
        return result;
    } ();
    
    static const auto obseWorkspace = []() {
        if (pluginPath.extension() == ".dll") {
            auto parent = pluginPath.parent_path();
            if (Utility::PathEndsWith(parent, "OBSE\\Plugins")) {
                return true;
            }
        }
        return false;
    } ();

}
