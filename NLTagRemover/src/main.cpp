namespace PluginData {
    static const std::string pluginName = "NLTagRemover";
    static const std::string iniFilename = std::format("{}.ini", pluginName);
    static const std::string logFilename = std::format("{}.log", pluginName);
    static const spdlog::level::level_enum logLevel = spdlog::level::info;
}

std::size_t ReadUInt64(std::string section, std::string key, std::size_t fallback) {
    mINI::INIFile file(PluginData::iniFilename);
    mINI::INIStructure ini;
    if (file.read(ini)) if (ini.has(section)) if (auto map = ini.get(section); map.has(key)) {
        auto value = map.get(key);
        try {
            return std::stoull(value, nullptr, 0);
        } catch (...) {
            // do nothing
        }
    }
    return fallback;
}

bool RemoveNLTag() {
    auto offset = ReadUInt64("General", "uOffset", 0x7DABBAC); // game version 0.411.140
    LOG("Memory Offset = 0x{:X}", offset);
    REL::Relocation<const char*> target{ REL::Offset(offset) };
    std::size_t count = 4;
    if (std::strncmp(target.get(), "[NL]", count)) return false;
    REL::WriteSafeFill(target.address(), 0, count);
    return true;
}

auto SetupSpdlog(std::string pattern) {
    auto logger = spdlog::basic_logger_mt(PluginData::pluginName, PluginData::logFilename, true);
    spdlog::set_default_logger(logger);
    spdlog::set_level(PluginData::logLevel);
    spdlog::set_pattern(pattern);
    logger->flush_on(PluginData::logLevel);
    return logger;
}

std::string GetExeVersion() {
    wchar_t path[1024];
    REX::W32::GetModuleFileNameW(NULL, path, sizeof(path));
    auto version = REL::GetFileVersion(path);
    return (version.has_value() ? version.value().string(".") : "Unknown");
}

void Startup() {
    auto logger = SetupSpdlog("%d.%m.%Y %H:%M:%S [%t] %v");
    LOG("ASI Plugin Loaded");
    LOG("Game Version = {}", GetExeVersion());
    LOG("NL-Tag Patch {}", RemoveNLTag() ? "Applied" : "Failed");
}

OBRE_DLL(reason) {
    if (reason == 1) {
        std::thread(Startup).detach();
    }
    return 1;
}