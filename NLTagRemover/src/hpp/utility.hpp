#pragma once

namespace Utility {

    bool logReady = false;

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

    void SetupSpdlog(std::string name, fs::path path, std::string pattern) {
        spdlog::level::level_enum logLevel = spdlog::level::info;
        auto logger = spdlog::basic_logger_mt(name, path.string(), true);
        spdlog::set_default_logger(logger);
        spdlog::set_level(logLevel);
        spdlog::flush_on(logLevel);
        spdlog::set_pattern(pattern);
        logReady = true;
    }

}
