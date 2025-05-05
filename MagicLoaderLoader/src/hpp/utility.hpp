#pragma once

namespace Utility {

    extern "C" OBSE::PluginVersionData OBSEPlugin_Version;

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

    static const auto WorkingDirectory = []() {
        // tweak compatibility
        OBSEPlugin_Version.UsesAddressLibrary(false);
        OBSEPlugin_Version.UsesSigScanning(true);
        OBSEPlugin_Version.IsLayoutDependent(false);
        OBSEPlugin_Version.HasNoStructUse(true);
        // set new root
        return fs::path(Utility::GetModulePathSafe(NULL)).parent_path();
    }();

}