#include "hpp/plugin.hpp"
#include "hpp/game.hpp"

bool RemoveNLTag() {
    const std::size_t count = 5;
    const std::uint8_t tag[count] = "[NL]";
    const auto tagArray = std::to_array(tag);
    auto offset = Game::ModuleOffset<count>(tagArray);
    AIX_LOG("Offset value = 0x{:X}", offset);
    if (offset == 0) return false;
    REL::Relocation target{ Game::moduleAddress + offset };
    REL::WriteSafeFill(target.address(), 0, count);
    return true;
}

void Startup(std::string mode) {
    Utility::SetupAixLog(Plugin::logPath, "%d.%m.%Y %H:%M:%S [#function] #message");
    AIX_LOG("{} version = {} / {}", Plugin::pluginName, Plugin::pluginVersion, mode);
    AIX_LOG("Game version = {}", Game::moduleVersion);
    AIX_LOG("Tag patch {}", RemoveNLTag() ? "applied" : "failed");
}

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* obseLoad) {
    if (OBSE::Init(obseLoad, { false }); obseLoad->GetReleaseIndex() > 0) Startup("DLL");
    return true;
}

OB64_DLL(const std::uint32_t reason) {
    if (reason == 1 && Plugin::obseWorkspace == false) std::thread(Startup, "ASI").detach();
    return 1;
}
