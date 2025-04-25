#pragma once
#include "hpp/utility.hpp"

namespace Game {

    std::wstring ModulePath() {
        return Utility::GetModulePathSafe(NULL);
    }

    std::string ModuleVersion() {
        auto filePath = ModulePath();
        auto version = REL::GetFileVersion(filePath);
        return (version.has_value() ? version.value().string(".") : "Unknown");
    }

    HMODULE ModuleHandle() {
        auto filePath = ModulePath();
        return GetModuleHandleW(filePath.data());
    }

    std::uintptr_t ModuleAddress() {
        auto handle = ModuleHandle();
        return reinterpret_cast<std::uintptr_t>(handle);
    }

    template<unsigned long long S>
    std::ptrdiff_t ModuleOffset(std::array<std::uint8_t, S> bytes) {
        auto data = bytes.data();
        auto hModule = ModuleHandle();
        auto pModule = ModuleAddress();
        MODULEINFO info;
        K32GetModuleInformation(GetCurrentProcess(), hModule, &info, sizeof(info));
        std::ptrdiff_t offsetMax = info.SizeOfImage - S;
        for (std::ptrdiff_t offset = 0; offset <= offsetMax; offset++)
            if (std::memcmp(reinterpret_cast<decltype(data)>(pModule + offset), data, S) == 0)
                return offset;
        // not found
        return 0;
    }

}
