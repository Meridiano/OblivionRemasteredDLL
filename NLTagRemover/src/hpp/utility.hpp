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
                return L"";
            } else if (copied < size - 1) {
                buffer.resize(copied);
                return buffer;
            }
            size *= 2;
        }
    }

    void SetupAixLog(fs::path path, std::string pattern) {
        auto sinkFile = std::make_shared<AixLog::SinkFile>(
            AixLog::Severity::info,
            path.string(),
            pattern
        );
        std::vector<AixLog::log_sink_ptr> sinks{ sinkFile };
        AixLog::Log::init(sinks);
        logReady = true;
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

}
