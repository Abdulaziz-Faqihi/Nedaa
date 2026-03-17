#pragma once
#include <string>

class DataManager {
public:
    static DataManager& Instance();

    // Check if world data (countries.json, states.json) is cached
    bool IsWorldDataCached() const;

    // Download world data from GitHub to local cache
    // Returns true if successful
    bool DownloadWorldData();

    // Get the data cache directory (%LOCALAPPDATA%\Nidaa\data\)
    std::wstring GetCacheDir() const;

    // Get the exe directory (where bundled Saudi data lives)
    std::wstring GetExeDir() const;

private:
    DataManager();
    bool DownloadFile(const wchar_t* url, const wchar_t* localPath);
    std::wstring m_cacheDir;
    std::wstring m_exeDir;
};
