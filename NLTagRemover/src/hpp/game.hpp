#pragma once
#include "hpp/utility.hpp"

namespace Game {

    static const auto modulePath = Utility::GetModulePathSafe(NULL);

    static const auto moduleHandle = GetModuleHandleW(modulePath.data());

    static const auto moduleAddress = reinterpret_cast<std::uintptr_t>(moduleHandle);

    static const auto moduleVersion = []() {
        auto version = REL::GetFileVersion(modulePath);
        return (version.has_value() ? version.value().string(".") : "Unknown");
    } ();

    template<std::size_t size>
    std::ptrdiff_t ModuleOffset(std::array<std::uint8_t, size> bytes) {
        auto data = bytes.data();
        MODULEINFO info;
        K32GetModuleInformation(GetCurrentProcess(), moduleHandle, &info, sizeof(info));
        std::ptrdiff_t offsetMax = info.SizeOfImage - size;
        for (std::ptrdiff_t offset = 0; offset <= offsetMax; offset++)
            if (std::memcmp(reinterpret_cast<decltype(data)>(moduleAddress + offset), data, size) == 0)
                return offset;
        // not found
        return 0;
    }

}
