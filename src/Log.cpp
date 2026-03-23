#include "Log.h"
#include <windows.h>
#include <shlobj.h>
#include <fstream>
#include <ctime>
#include <mutex>

static std::wstring g_logPath;
static std::mutex g_logMutex;

static std::wstring GetTimestamp() {
    time_t now = time(nullptr);
    struct tm lt;
    localtime_s(&lt, &now);
    wchar_t buf[64];
    swprintf_s(buf, L"%04d-%02d-%02d %02d:%02d:%02d",
        lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday,
        lt.tm_hour, lt.tm_min, lt.tm_sec);
    return buf;
}

void Log::Init() {
    PWSTR localAppData = nullptr;
    std::wstring dir;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &localAppData))) {
        dir = localAppData;
        CoTaskMemFree(localAppData);
    } else {
        dir = L".";
    }
    dir += L"\\Nidaa";
    CreateDirectoryW(dir.c_str(), nullptr);
    dir += L"\\logs";
    CreateDirectoryW(dir.c_str(), nullptr);
    g_logPath = dir + L"\\nidaa.log";

    // Truncate if log exceeds 1MB
    DWORD attr = GetFileAttributesW(g_logPath.c_str());
    if (attr != INVALID_FILE_ATTRIBUTES) {
        WIN32_FILE_ATTRIBUTE_DATA fad;
        if (GetFileAttributesExW(g_logPath.c_str(), GetFileExInfoStandard, &fad)) {
            ULARGE_INTEGER size;
            size.LowPart = fad.nFileSizeLow;
            size.HighPart = fad.nFileSizeHigh;
            if (size.QuadPart > 1024 * 1024) {
                DeleteFileW(g_logPath.c_str());
            }
        }
    }

    Info(L"--- App started ---");
}

static void WriteLog(const std::wstring& level, const std::wstring& msg) {
    if (g_logPath.empty()) return;
    std::lock_guard<std::mutex> lock(g_logMutex);

    // Convert to UTF-8 for file
    std::wstring line = L"[" + GetTimestamp() + L"] [" + level + L"] " + msg + L"\n";
    int needed = WideCharToMultiByte(CP_UTF8, 0, line.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (needed <= 0) return;
    std::string utf8(needed - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, line.c_str(), -1, utf8.data(), needed, nullptr, nullptr);

    std::ofstream f(g_logPath, std::ios::app | std::ios::binary);
    if (f.is_open()) {
        f.write(utf8.data(), utf8.size());
    }
}

void Log::Info(const std::wstring& msg) {
    WriteLog(L"INFO", msg);
}

void Log::Error(const std::wstring& msg) {
    WriteLog(L"ERROR", msg);
}

std::wstring Log::GetLogFilePath() {
    return g_logPath;
}

std::string Log::ReadLogFileUtf8() {
    if (g_logPath.empty()) return "";
    std::lock_guard<std::mutex> lock(g_logMutex);
    std::ifstream f(g_logPath, std::ios::binary);
    if (!f.is_open()) return "";
    return std::string((std::istreambuf_iterator<char>(f)),
                        std::istreambuf_iterator<char>());
}
