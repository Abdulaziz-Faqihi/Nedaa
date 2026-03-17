#include "DataManager.h"

#include <windows.h>
#include <shlobj.h>
#include <urlmon.h>

#pragma comment(lib, "urlmon.lib")

DataManager& DataManager::Instance()
{
    static DataManager instance;
    return instance;
}

DataManager::DataManager()
{
    // Compute cache directory
    PWSTR localAppData = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &localAppData))) {
        m_cacheDir = localAppData;
        CoTaskMemFree(localAppData);
    }
    else {
        // Fallback to a relative path if SHGetKnownFolderPath fails
        m_cacheDir = L".";
    }
    m_cacheDir += L"\\Nidaa";
    CreateDirectoryW(m_cacheDir.c_str(), nullptr);
    m_cacheDir += L"\\data";
    CreateDirectoryW(m_cacheDir.c_str(), nullptr);
    m_cacheDir += L"\\";

    // Compute exe directory
    wchar_t exePath[MAX_PATH] = {};
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    // Strip the filename to get the directory
    std::wstring exePathStr(exePath);
    auto pos = exePathStr.find_last_of(L"\\/");
    if (pos != std::wstring::npos)
        m_exeDir = exePathStr.substr(0, pos + 1);
    else
        m_exeDir = L".\\";
}

bool DataManager::IsWorldDataCached() const
{
    std::wstring countriesPath = m_cacheDir + L"countries.json";
    std::wstring statesPath = m_cacheDir + L"states.json";

    DWORD attrC = GetFileAttributesW(countriesPath.c_str());
    DWORD attrS = GetFileAttributesW(statesPath.c_str());

    return (attrC != INVALID_FILE_ATTRIBUTES && !(attrC & FILE_ATTRIBUTE_DIRECTORY))
        && (attrS != INVALID_FILE_ATTRIBUTES && !(attrS & FILE_ATTRIBUTE_DIRECTORY));
}

bool DataManager::DownloadWorldData()
{
    // Copy bundled countries.json and states.json from exe dir to cache
    std::wstring srcCountries = m_exeDir + L"countries.json";
    std::wstring dstCountries = m_cacheDir + L"countries.json";
    if (!CopyFileW(srcCountries.c_str(), dstCountries.c_str(), FALSE))
        return false;

    std::wstring srcStates = m_exeDir + L"states.json";
    std::wstring dstStates = m_cacheDir + L"states.json";
    if (!CopyFileW(srcStates.c_str(), dstStates.c_str(), FALSE))
        return false;

    // Download Saudi data from GitHub
    const wchar_t* regionsUrl =
        L"https://raw.githubusercontent.com/homaily/Saudi-Arabia-Regions-Cities-and-Districts/refs/heads/master/json/regions_lite.json";
    std::wstring regionsPath = m_cacheDir + L"sa_regions_lite.json";
    if (!DownloadFile(regionsUrl, regionsPath.c_str()))
        return false;

    const wchar_t* citiesUrl =
        L"https://raw.githubusercontent.com/homaily/Saudi-Arabia-Regions-Cities-and-Districts/refs/heads/master/json/cities.json";
    std::wstring citiesPath = m_cacheDir + L"sa_cities.json";
    if (!DownloadFile(citiesUrl, citiesPath.c_str()))
        return false;

    return true;
}

std::wstring DataManager::GetCacheDir() const
{
    return m_cacheDir;
}

std::wstring DataManager::GetExeDir() const
{
    return m_exeDir;
}

bool DataManager::DownloadFile(const wchar_t* url, const wchar_t* localPath)
{
    HRESULT hr = URLDownloadToFileW(nullptr, url, localPath, 0, nullptr);
    return SUCCEEDED(hr);
}
