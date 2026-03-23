#pragma once
#include <string>

struct AppConfig {
    std::string countryIso2 = "SA";
    int stateId = 0;
    int saudiCityId = 6;           // Only used when country is SA
    int hotkeyModifiers = 0x0006;  // MOD_CONTROL | MOD_ALT
    int hotkeyVK = 'P';
    int language = 0;              // 0=AR, 1=EN
    int calcMethod = -1;           // -1=Auto (from country), 0-7=specific method
};

bool ConfigLoad(AppConfig& config);
bool ConfigSave(const AppConfig& config);
std::wstring ConfigGetPath();
bool IsFirstLaunch();
