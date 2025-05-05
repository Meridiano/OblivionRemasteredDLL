#include "hpp/utility.hpp"

bool CheckFiles() {
    bool result = false;
    // check output first
    fs::path outputFolder = RELATIVE("../../Content/Paks/~mods/MagicLoader", outputFolder);
    if (fs::exists(outputFolder) && fs::is_directory(outputFolder)) {
        std::size_t filesCount = [](fs::path path) {
            using fs::directory_iterator;
            using fp = bool(*)(const std::filesystem::path&);
            return std::count_if(directory_iterator(path), directory_iterator{}, (fp)fs::is_regular_file);
        }(outputFolder);
        if (filesCount == 0) {
            REX::INFO(LAUNCH_REQUIRED, "Output", "Files");
            result |= true;
        }
    } else {
        REX::INFO(LAUNCH_REQUIRED, "Output", "Folder");
        result |= true;
    }
    std::string keyName = "iUnixTime";
    fs::path storageFile = RELATIVE(std::format("OBSE/Plugins/{}.ini", OBSE::GetPluginName()), storageFile);
    // read storage
    mINI::INIFile file(storageFile);
    mINI::INIStructure ini;
    if (file.read(ini)) {
        // create new ini structure
        mINI::INIStructure temp;
        // populate temp structure
        std::string configFolderString = "../../Content/Dev/ObvData/Data/MagicLoader";
        fs::path configFolder = RELATIVE(configFolderString, configFolder);
        if (fs::exists(configFolder) && fs::is_directory(configFolder)) for (auto entry : fs::recursive_directory_iterator(configFolder)) {
            auto entryPath = entry.path();
            if (entry.is_regular_file() && entryPath.extension() == ".json") {
                auto entryPathString = fs::relative(entryPath, configFolder).string();
                auto timeDuration = fs::last_write_time(entryPath).time_since_epoch();
                auto timeInteger = std::chrono::duration_cast<std::chrono::seconds>(timeDuration).count();
                auto timeString = std::format("{}", timeInteger);
                // log and push
                REX::INFO("Json = {}:{}", entryPathString, timeString);
                temp[entryPathString][keyName] = timeString;
            }
        }
        // ini compare lambda
        auto IniCompare = [](mINI::INIStructure iniA, mINI::INIStructure iniB, std::string key) {
            for (auto iterator : iniA) {
                if (auto section = iterator.first; iniB.has(section)) {
                    if (iniA.get(section).get(key) != iniB.get(section).get(key)) {
                        // different key values
                        REX::INFO(LAUNCH_REQUIRED, "JsonTime", section);
                        return true;
                    }
                } else {
                    // iniB has no section from iniA
                    REX::INFO(LAUNCH_REQUIRED, "JsonMissing", section);
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
        if (generated) REX::INFO("Storage refreshed");
        else CRITICAL("Failed to refresh storage");
    } else {
        REX::INFO(LAUNCH_REQUIRED, "Storage", "Disabled");
        result |= true;
    }
    return result;
}

void PluginProcess() {
    bool needLaunch = CheckFiles();
    if (needLaunch) {
        fs::path magicLoader = RELATIVE("../../../MagicLoader/mlcli.exe", magicLoader);
        std::string command = std::format("cmd.exe /c \"{}\"", magicLoader.string());
        auto t1 = std::chrono::steady_clock::now();
        bool result = Utility::RunCommandAndWait(OBSE::stl::utf8_to_utf16(command).value_or(L""));
        auto t2 = std::chrono::steady_clock::now();
        if (result) REX::INFO("Total time = {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
        else CRITICAL("Failed to launch command");
    } else REX::INFO("No reason to launch");
}

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* a_obse) {
    OBSE::Init(a_obse);
    PluginProcess();
    return true;
}
