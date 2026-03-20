#include "Updater.h"
#include <windows.h>
#include <wininet.h>
#include <urlmon.h>
#include <shlobj.h>
#include <string>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "urlmon.lib")

static bool IsNewerVersion(const std::string& latest, const std::string& current) {
    int lMajor = 0, lMinor = 0, lPatch = 0;
    int cMajor = 0, cMinor = 0, cPatch = 0;
    sscanf_s(latest.c_str(), "%d.%d.%d", &lMajor, &lMinor, &lPatch);
    sscanf_s(current.c_str(), "%d.%d.%d", &cMajor, &cMinor, &cPatch);

    if (lMajor != cMajor) return lMajor > cMajor;
    if (lMinor != cMinor) return lMinor > cMinor;
    return lPatch > cPatch;
}

// Extract a JSON string value by key (simple parser, no full JSON lib needed)
static std::string ExtractJsonString(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\"";
    auto pos = json.find(search);
    if (pos == std::string::npos) return "";

    pos += search.size();
    // Skip : and whitespace
    while (pos < json.size() && (json[pos] == ':' || json[pos] == ' ' || json[pos] == '\t')) pos++;
    // Skip opening quote
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;

    auto end = json.find('"', pos);
    if (end == std::string::npos) return "";
    return json.substr(pos, end - pos);
}

static std::string HttpGet(const wchar_t* host, const wchar_t* path) {
    std::string result;

    HINTERNET hInternet = InternetOpenW(L"Nidaa Updater", INTERNET_OPEN_TYPE_PRECONFIG,
                                         nullptr, nullptr, 0);
    if (!hInternet) return result;

    HINTERNET hConnect = InternetConnectW(hInternet, host, INTERNET_DEFAULT_HTTPS_PORT,
                                           nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return result;
    }

    DWORD flags = INTERNET_FLAG_SECURE | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD;
    HINTERNET hRequest = HttpOpenRequestW(hConnect, L"GET", path, nullptr, nullptr,
                                           nullptr, flags, 0);
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return result;
    }

    if (HttpSendRequestW(hRequest, nullptr, 0, nullptr, 0)) {
        char buffer[4096];
        DWORD bytesRead = 0;
        while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            result += buffer;
        }
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return result;
}

bool Updater::CheckForUpdate(std::wstring& installerPath) {
    // GitHub API: get latest release
    std::string response = HttpGet(
        L"api.github.com",
        L"/repos/Abdulaziz-Faqihi/Nedaa/releases/latest"
    );
    if (response.empty()) return false;

    // Extract version from tag_name
    std::string tagName = ExtractJsonString(response, "tag_name");
    if (tagName.empty()) return false;

    // Strip 'v' prefix if present
    std::string version = tagName;
    if (!version.empty() && version[0] == 'v') version = version.substr(1);

    if (!IsNewerVersion(version, NIDAA_VERSION)) return false;

    // Find the installer asset URL (browser_download_url for .exe file)
    // Look for "browser_download_url" followed by a URL ending in .exe
    std::string searchKey = "\"browser_download_url\":";
    size_t pos = 0;
    std::string assetUrl;
    while ((pos = response.find(searchKey, pos)) != std::string::npos) {
        pos += searchKey.size();
        while (pos < response.size() && (response[pos] == ' ' || response[pos] == '"')) pos++;
        auto end = response.find('"', pos);
        if (end == std::string::npos) break;

        std::string url = response.substr(pos, end - pos);
        // Check if this is an .exe asset
        if (url.size() > 4 && url.substr(url.size() - 4) == ".exe") {
            assetUrl = url;
            break;
        }
        pos = end;
    }

    if (assetUrl.empty()) return false;

    // Build temp file path: %TEMP%\NidaaSetup.exe
    wchar_t tempDir[MAX_PATH] = {};
    GetTempPathW(MAX_PATH, tempDir);
    installerPath = tempDir;
    installerPath += L"NidaaSetup.exe";

    // Download the installer
    std::wstring wUrl(assetUrl.begin(), assetUrl.end());
    HRESULT hr = URLDownloadToFileW(nullptr, wUrl.c_str(), installerPath.c_str(), 0, nullptr);
    if (FAILED(hr)) return false;

    return true;
}
