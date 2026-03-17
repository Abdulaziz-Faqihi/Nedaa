#include "Config.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <windows.h>
#include <shlobj.h>

using json = nlohmann::json;

std::wstring ConfigGetPath() {
    wchar_t* appData = nullptr;
    SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &appData);
    std::wstring path(appData);
    CoTaskMemFree(appData);
    path += L"\\Nidaa";
    CreateDirectoryW(path.c_str(), nullptr);
    path += L"\\config.json";
    return path;
}

bool IsFirstLaunch() {
    std::wstring path = ConfigGetPath();
    DWORD attrs = GetFileAttributesW(path.c_str());
    return (attrs == INVALID_FILE_ATTRIBUTES);
}

bool ConfigLoad(AppConfig& config) {
    std::wstring path = ConfigGetPath();
    std::ifstream f(path);
    if (!f.is_open()) {
        return false;
    }

    try {
        json j = json::parse(f);
        if (j.contains("countryIso2"))
            config.countryIso2 = j["countryIso2"].get<std::string>();
        if (j.contains("stateId"))
            config.stateId = j["stateId"].get<int>();
        if (j.contains("saudiCityId"))
            config.saudiCityId = j["saudiCityId"].get<int>();
        if (j.contains("hotkeyMod"))
            config.hotkeyModifiers = j["hotkeyMod"].get<int>();
        if (j.contains("hotkeyVK"))
            config.hotkeyVK = j["hotkeyVK"].get<int>();
        if (j.contains("language"))
            config.language = j["language"].get<int>();
    } catch (...) {
        return false;
    }
    return true;
}

bool ConfigSave(const AppConfig& config) {
    json j;
    j["countryIso2"] = config.countryIso2;
    j["stateId"] = config.stateId;
    j["saudiCityId"] = config.saudiCityId;
    j["hotkeyMod"] = config.hotkeyModifiers;
    j["hotkeyVK"] = config.hotkeyVK;
    j["language"] = config.language;

    std::wstring path = ConfigGetPath();
    std::ofstream f(path);
    if (!f.is_open()) return false;
    f << j.dump(2);
    return true;
}
