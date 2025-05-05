#include "hpp/plugin.hpp"

bool CheckFiles() {
    bool result = false;
    // check output first
    fs::path outputFolder = RELATIVE("../../Content/Paks/~mods/MagicLoader");
    if (fs::exists(outputFolder) && fs::is_directory(outputFolder)) {
        std::size_t filesCount = [](fs::path path) {
            using fs::directory_iterator;
            using fp = bool(*)(const fs::path&);
            return std::count_if(directory_iterator(path), directory_iterator{}, (fp)fs::is_regular_file);
        }(outputFolder);
        if (filesCount == 0) {
            LOG(LAUNCH_REQUIRED, "Output", "Files");
            result |= true;
        }
    } else {
        LOG(LAUNCH_REQUIRED, "Output", "Folder");
        result |= true;
    }
    std::string keyName = "iUnixTime";
    fs::path storageFile = Plugin::iniPath;
    // read storage
    mINI::INIFile file(storageFile);
    mINI::INIStructure ini;
    if (file.read(ini)) {
        // create new ini structure
        mINI::INIStructure temp;
        // populate temp structure
        fs::path configFolder = RELATIVE("../../Content/Dev/ObvData/Data/MagicLoader");
        if (fs::exists(configFolder) && fs::is_directory(configFolder)) for (auto entry : fs::recursive_directory_iterator(configFolder)) {
            auto entryPath = entry.path();
            if (entry.is_regular_file() && entryPath.extension() == ".json") {
                auto entryPathString = fs::relative(entryPath, configFolder).string();
                auto timeInteger = Utility::FileTimeStamp(entryPath);
                auto timeString = std::format("{}", timeInteger);
                // log and push
                LOG("Json = {}:{}", entryPathString, timeString);
                temp[entryPathString][keyName] = timeString;
            }
        }
        // ini compare lambda
        auto IniCompare = [](mINI::INIStructure iniA, mINI::INIStructure iniB, std::string key) {
            for (auto iterator : iniA) {
                if (auto section = iterator.first; iniB.has(section)) {
                    if (iniA.get(section).get(key) != iniB.get(section).get(key)) {
                        // different key values
                        LOG(LAUNCH_REQUIRED, "JsonTime", section);
                        return true;
                    }
                } else {
                    // iniB has no section from iniA
                    LOG(LAUNCH_REQUIRED, "JsonFile", section);
                    return true;
                }
            }
            return false;
        };
        // compare
        bool cmpA = IniCompare(ini, temp, keyName);
        bool cmpB = IniCompare(temp, ini, keyName);
        result |= (cmpA || cmpB);
        // refresh storage
        bool generated = file.generate(temp, true);
        if (generated) {
            LOG("Storage refreshed");
        } else {
            CRITICAL("Failed to refresh storage");
        }
    } else {
        LOG(LAUNCH_REQUIRED, "Storage", "Disabled");
        result |= true;
    }
    return result;
}

void PluginProcess(std::string mode) {
    Utility::SetupSpdlog(Plugin::pluginName, Plugin::logPath, "%d.%m.%Y %H:%M:%S [%t] %v");
    LOG("{} version = {} / {}", Plugin::pluginName, Plugin::pluginVersion, mode);
    bool needLaunch = CheckFiles();
    if (needLaunch) {
        fs::path magicLoader = RELATIVE("../../../MagicLoader/mlcli.exe");
        std::string command = std::format("cmd.exe /c {}", Utility::Quoted(magicLoader.string(), '"'));
        auto t1 = std::chrono::steady_clock::now();
        bool result = Utility::RunCommandAndWait(OBSE::stl::utf8_to_utf16(command).value_or(L""));
        auto t2 = std::chrono::steady_clock::now();
        if (result) {
            LOG("Total time = {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
        } else {
            CRITICAL("Failed to launch command");
        }
    } else {
        LOG("No reason to launch");
    }
}

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* a_obse) {
    OBSE::InitInfo info = { .log = false };
    OBSE::Init(a_obse, info);
    PluginProcess("DLL");
    return true;
}

OB64_DLL(const std::uint32_t reason) {
    if (reason == 1)
        if (Plugin::obseWorkspace == false)
            PluginProcess("ASI");
    return 1;
}