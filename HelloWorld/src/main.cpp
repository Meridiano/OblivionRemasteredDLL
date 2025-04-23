void Startup() {
    auto version = REL::GetFileVersion(L"OblivionRemastered-Win64-Shipping.exe"sv);
    auto versionString = version.has_value() ? version.value().string(".") : "Unknown";
    
    REL::Relocation<const char*> game{ REL::Offset(0x6E744B0) };
    auto gameString = std::string(game.get());

    auto message = std::format("{} version {}", gameString, versionString);
    REX::W32::MessageBoxA(nullptr, message.data(), "Hello World", 0x40);
}

OBRE_DLL(reason) {
    if (reason == 1) {
        std::thread(Startup).detach();
    }
    return 1;
}