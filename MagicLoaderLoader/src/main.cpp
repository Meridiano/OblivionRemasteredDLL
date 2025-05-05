namespace PluginCompatibility {

    extern "C" OBSE::PluginVersionData OBSEPlugin_Version;
    static const auto pluginName = []() {
        OBSEPlugin_Version.UsesAddressLibrary(false);
        OBSEPlugin_Version.UsesSigScanning(true);
        OBSEPlugin_Version.IsLayoutDependent(false);
        OBSEPlugin_Version.HasNoStructUse(true);
        return OBSEPlugin_Version.pluginName;
    }();
    
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

std::string DecodeBase64(const std::string base64) {
    DWORD binarySize = 0;
    if (CryptStringToBinaryA(base64.data(), 0, CRYPT_STRING_BASE64, 0, &binarySize, 0, 0)) {
        std::vector<BYTE> binary(binarySize);
        if (CryptStringToBinaryA(base64.data(), 0, CRYPT_STRING_BASE64, binary.data(), &binarySize, 0, 0)) {
            return std::string(reinterpret_cast<char*>(binary.data()), binarySize);
        }
    }
    return std::string("");
}

bool CheckFiles() {
    // define things
    bool result = false;
    std::string keyName = "iUnixTime";
    fs::path storagePath = std::format("OBSE/Plugins/{}.ini", OBSE::GetPluginName());
    // read storage
    mINI::INIFile file(storagePath);
    mINI::INIStructure ini;
    if (file.read(ini)) {
        // create new ini structure
        mINI::INIStructure temp;
        // populate temp structure
        std::string configPathString = "../../Content/Dev/ObvData/Data/MagicLoader";
        std::size_t prefixSize = configPathString.length() + 1;
        fs::path configPath = configPathString;
        if (fs::exists(configPath) && fs::is_directory(configPath)) for (auto entry : fs::recursive_directory_iterator(configPath)) {
            auto entryPath = entry.path();
            if (entry.is_regular_file() && entryPath.extension() == ".json") {
                auto entryPathString = entryPath.string();
                auto entryPathStringPart = entryPathString.substr(prefixSize, std::string::npos);
                auto timeDuration = fs::last_write_time(entryPath).time_since_epoch();
                auto timeInteger = std::chrono::duration_cast<std::chrono::seconds>(timeDuration).count();
                auto timeString = std::format("{}", timeInteger);
                // log and push
                REX::INFO("Json = {}:{}", entryPathStringPart, timeString);
                temp[entryPathStringPart][keyName] = timeString;
            }
        }
        // ini compare lambda
        auto IniCompare = [](mINI::INIStructure iniA, mINI::INIStructure iniB, std::string key) {
            for (auto iterator : iniA) {
                if (auto section = iterator.first; iniB.has(section)) {
                    if (iniA.get(section).get(key) != iniB.get(section).get(key)) {
                        // different key values
                        REX::INFO("Launch required, reason.value = {}", section);
                        return true;
                    }
                } else {
                    // iniB has no section from iniA
                    REX::INFO("Launch required, reason.match = {}", section);
                    return true;
                }
            }
            return false;
        };
        // compare
        bool cmpA = IniCompare(ini, temp, keyName);
        bool cmpB = IniCompare(temp, ini, keyName);
        result = (cmpA || cmpB);
        // refresh storage
        bool generated = file.generate(temp, true);
        if (generated) REX::INFO("Storage refreshed");
        else CRITICAL("Failed to refresh storage");
    } else CRITICAL("Unable to read {} storage", storagePath.string());
    return result;
}

void PluginProcess() {
    bool needLaunch = CheckFiles();
    if (needLaunch) {
        std::string command = DecodeBase64("Y21kLmV4ZSAvYyAiLi5cLi5cLi5cbWFnaWNsb2FkZXJcbWxjbGkuZXhlIg");
        REX::INFO("Launch command = {}", command);
        auto t1 = std::chrono::steady_clock::now();
        bool result = RunCommandAndWait(OBSE::stl::utf8_to_utf16(command).value_or(L""));
        auto t2 = std::chrono::steady_clock::now();
        if (result) REX::INFO("Total time = {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
        else CRITICAL("Failed to launch command");
    } else REX::INFO("No changes in configs");
}

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* a_obse) {
    OBSE::Init(a_obse);
    PluginProcess();
    return true;
}
