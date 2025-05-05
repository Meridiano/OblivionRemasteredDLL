#pragma once

namespace Utility {

    bool logReady = false;

    std::wstring GetModulePathSafe(HMODULE hModule) {
        std::uint32_t size = MAX_PATH;
        std::wstring buffer;
        while (true) {
            buffer.resize(size);
            std::uint32_t copied = GetModuleFileNameW(hModule, &buffer[0], size);
            if (copied == 0) {
                buffer.clear();
                break;
            } else if (copied < size - 1) {
                buffer.resize(copied);
                break;
            }
            size *= 2;
        }
        return buffer;
    }

    void SetupSpdlog(std::string name, fs::path path, std::string pattern) {
        auto logger = spdlog::basic_logger_mt(name, path.string(), true);
        auto logLevel = spdlog::level::info;
        spdlog::set_default_logger(logger);
        logger->set_pattern(pattern);
        logger->set_level(logLevel);
        logger->flush_on(logLevel);
        logReady = true;
    }

    bool RunCommandAndWait(const std::wstring command) {
        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        bool success = CreateProcessW(0, LPWSTR(command.data()), 0, 0, 0, 0, 0, 0, &si, &pi);
        if (success) {
            WaitForSingleObject(pi.hProcess, std::numeric_limits<std::uint32_t>::max());
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        return success;
    }

    bool PathEndsWith(fs::path full, fs::path ends) {
        auto ReversedView = [](fs::path p) {
            std::vector<fs::path> v = { p.begin(), p.end() };
            return (v | std::views::reverse);
        };
        auto fullView = ReversedView(full);
        auto endsView = ReversedView(ends);
        return std::ranges::equal(endsView, fullView);
    }

    auto FileTimeStamp(fs::path path) {
        auto timeWindows = fs::last_write_time(path).time_since_epoch();
        auto timeInteger = std::chrono::duration_cast<std::chrono::seconds>(timeWindows).count();
        return (timeInteger - 11644473600);
    }

    std::string Quoted(std::string str, char del) {
        std::stringstream ss;
        ss << std::quoted(str, del);
        return ss.str();
    }

    static const auto workingDirectory = fs::path(GetModulePathSafe(NULL)).parent_path();

}