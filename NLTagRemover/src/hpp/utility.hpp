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
            } else if (copied < size) {
                buffer.resize(copied);
                break;
            }
            size *= 2;
        }
        return buffer;
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
        auto ReversedVector = [](fs::path p) {
            std::vector<fs::path> v = { p.begin(), p.end() };
            std::ranges::reverse(v);
            return v;
        };
        auto fullVector = ReversedVector(full);
        auto endsVector = ReversedVector(ends);
        return std::ranges::starts_with(fullVector, endsVector, [](const fs::path& a, const fs::path& b) {
            const auto& as = a.wstring();
            const auto& bs = b.wstring();
            return std::ranges::equal(as, bs, [](wchar_t x, wchar_t y) {
                return towlower(x) == towlower(y);
            });
        });
    }

}
