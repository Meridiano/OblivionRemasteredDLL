std::wstring GetModulePathSafe(HMODULE hModule) {
    DWORD size = MAX_PATH;
    std::wstring buffer;
    while (true) {
        buffer.resize(size);
        DWORD copied = GetModuleFileNameW(hModule, &buffer[0], size);
        if (copied == 0) {
            return L"";
        } else if (copied < size - 1) {
            buffer.resize(copied);
            return buffer;
        }
        size *= 2;
    }
}

namespace GameModule {

    std::wstring ModulePath() {
        return GetModulePathSafe(NULL);
    }

    std::string ModuleVersion() {
        auto filePath = ModulePath();
        auto version = REL::GetFileVersion(filePath);
        return (version.has_value() ? version.value().string(".") : "Unknown");
    }

    std::uintptr_t ModuleAddress() {
        auto filePath = ModulePath();
        auto handle = GetModuleHandleW(filePath.data());
        return reinterpret_cast<std::uintptr_t>(handle);
    }

}

namespace PluginData {

    static const std::string pluginName = "NLTagRemover";

    static const auto pluginPath = []() {
        HMODULE hModule = nullptr;
        GetModuleHandleExA(0x2 + 0x4, (LPCSTR)&pluginName, &hModule);
        return fs::path(GetModulePathSafe(hModule));
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

std::size_t ReadUInt64(std::string section, std::string key, std::size_t fallback) {
    LOG("Config path = {}", PluginData::iniPath.string());
    mINI::INIFile file(PluginData::iniPath);
    mINI::INIStructure ini;
    if (file.read(ini)) if (ini.has(section)) if (auto map = ini.get(section); map.has(key)) {
        auto value = map.get(key);
        try {
            return std::stoull(value, nullptr, 0);
        } catch (...) {
            // do nothing
        }
    }
    LOG("{}", "Offset override disabled");
    return fallback;
}

bool RemoveNLTag() {
    auto offset = ReadUInt64("General", "uOffset", 0x7DABBAC); // Steam version 0.411.140
    LOG("Offset value = 0x{:X}", offset);
    REL::Relocation<const char*> target{ GameModule::ModuleAddress() + std::ptrdiff_t(offset) };
    std::size_t count = 4;
    if (std::strncmp(target.get(), "[NL]", count)) return false;
    REL::WriteSafeFill(target.address(), 0, count);
    return true;
}

auto SetupSpdlog(std::string pattern) {
    auto logger = spdlog::basic_logger_mt(PluginData::pluginName, PluginData::logPath.string(), true);
    spdlog::set_default_logger(logger);
    spdlog::level::level_enum logLevel = spdlog::level::info;
    spdlog::set_level(logLevel);
    spdlog::flush_on(logLevel);
    spdlog::set_pattern(pattern);
    return logger;
}

void Startup() {
    auto logger = SetupSpdlog("%d.%m.%Y %H:%M:%S [%t] %v");
    LOG("{} version = {}", PluginData::pluginName, PluginData::pluginVersion);
    LOG("Game version = {}", GameModule::ModuleVersion());
    LOG("Tag patch {}", RemoveNLTag() ? "applied" : "failed");
}

OBRE_DLL(reason) {
    if (reason == 1) {
        std::thread(Startup).detach();
    }
    return 1;
}