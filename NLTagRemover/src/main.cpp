#include "hpp/plugin.hpp"
#include "hpp/game.hpp"

bool RemoveNLTag() {
    const std::size_t count = 5;
    const std::uint8_t tag[count] = "[NL]";
    const auto tagArray = std::to_array(tag);
    auto offset = Game::ModuleOffset<count>(tagArray);
    LOG("Offset value = 0x{:X}", offset);
    if (offset == 0) return false;
    REL::Relocation<const char*> target{ Game::ModuleAddress() + offset };
    REL::WriteSafeFill(target.address(), 0, count);
    return true;
}

void Startup() {
    Utility::SetupSpdlog(Plugin::pluginName, Plugin::logPath, "%d.%m.%Y %H:%M:%S [%t] %v");
    LOG("{} version = {}", Plugin::pluginName, Plugin::pluginVersion);
    LOG("Game version = {}", Game::ModuleVersion());
    LOG("Tag patch {}", RemoveNLTag() ? "applied" : "failed");
}

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* obseInterface) {
    if (OBSE::Init(obseInterface); obseInterface->GetReleaseIndex() > 0) Startup();
    return true;
}

OB64_DLL(const std::uint32_t reason) {
    if (reason == 1 && Plugin::obseWorkspace == false) std::thread(Startup).detach();
    return 1;
}
