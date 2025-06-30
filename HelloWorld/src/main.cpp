void Startup() {
    auto module = REL::Module::GetSingleton();
    auto moduleName = module ? module->filename() : L"?";

    auto version = REL::GetFileVersion(moduleName);
    auto versionString = version.has_value() ? version.value().string(".") : "?";

    auto gameString = REL::Relocation<const char*>{ REL::ID(522294) }.get();

    auto message = std::format("{} version {}", gameString, versionString);
    REX::W32::MessageBoxA(nullptr, message.data(), "Hello World", 0x40);
}

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* obse) {
    OBSE::Init(obse);
    std::thread(Startup).detach();
    return true;
}