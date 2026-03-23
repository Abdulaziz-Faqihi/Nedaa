#include "LogSender.h"
#include "Log.h"
#include "secrets.h"
#include "Updater.h"
#include <windows.h>
#include <wininet.h>
#include <string>
#include <vector>
#include <sstream>

#pragma comment(lib, "wininet.lib")

static std::string WideToUtf8(const std::wstring& wide) {
    if (wide.empty()) return "";
    int needed = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (needed <= 0) return "";
    std::string result(needed - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, result.data(), needed, nullptr, nullptr);
    return result;
}

bool LogSender::SendLogToTelegram(const std::wstring& userNote) {
    std::string logContent = Log::ReadLogFileUtf8();
    if (logContent.empty()) {
        Log::Error(L"SendLogToTelegram: log file is empty");
        return false;
    }

    // Prepend version and optional user note
    std::string header = "Nidaa v" NIDAA_VERSION "\n";
    if (!userNote.empty()) {
        header += "Note: " + WideToUtf8(userNote) + "\n";
    }
    header += "---\n";
    logContent = header + logContent;

    // Truncate to 4000 chars (Telegram message limit is 4096)
    if (logContent.size() > 4000) {
        logContent = logContent.substr(logContent.size() - 4000);
    }

    // Build the Telegram API URL
    std::string token = TELEGRAM_BOT_TOKEN;
    std::string url = "/bot" + token + "/sendMessage";

    // URL-encode the message text
    std::string escaped;
    for (char c : logContent) {
        if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped += c;
        } else {
            char buf[4];
            sprintf_s(buf, "%%%02X", (unsigned char)c);
            escaped += buf;
        }
    }

    // We need a chat_id. Use the TELEGRAM_CHAT_ID if set, otherwise use GetUpdates to find it.
    std::string chatId = TELEGRAM_CHAT_ID;

    if (chatId.empty()) {
        // Try to get chat_id from recent updates
        HINTERNET hInet = InternetOpenA("Nidaa", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
        if (!hInet) return false;

        std::string getUpdatesUrl = "/bot" + token + "/getUpdates?limit=1";
        HINTERNET hConn = InternetConnectA(hInet, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT,
                                           nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
        if (!hConn) { InternetCloseHandle(hInet); return false; }

        HINTERNET hReq = HttpOpenRequestA(hConn, "GET", getUpdatesUrl.c_str(), nullptr, nullptr,
                                          nullptr, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 0);
        if (!hReq) { InternetCloseHandle(hConn); InternetCloseHandle(hInet); return false; }

        if (HttpSendRequestA(hReq, nullptr, 0, nullptr, 0)) {
            std::string resp;
            char buf[4096];
            DWORD bytesRead;
            while (InternetReadFile(hReq, buf, sizeof(buf), &bytesRead) && bytesRead > 0) {
                resp.append(buf, bytesRead);
            }
            // Parse chat_id from response
            auto chatIdPos = resp.find("\"chat\":{\"id\":");
            if (chatIdPos != std::string::npos) {
                chatIdPos += 13;  // length of "\"chat\":{\"id\":"
                auto end = resp.find_first_of(",}", chatIdPos);
                if (end != std::string::npos) {
                    chatId = resp.substr(chatIdPos, end - chatIdPos);
                }
            }
        }
        InternetCloseHandle(hReq);
        InternetCloseHandle(hConn);
        InternetCloseHandle(hInet);

        if (chatId.empty()) {
            Log::Error(L"SendLogToTelegram: could not determine chat_id. Send /start to the bot first.");
            return false;
        }
    }

    // Send the message
    std::string postData = "chat_id=" + chatId + "&text=" + escaped;

    HINTERNET hInet = InternetOpenA("Nidaa", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    if (!hInet) return false;

    HINTERNET hConn = InternetConnectA(hInet, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT,
                                       nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConn) { InternetCloseHandle(hInet); return false; }

    HINTERNET hReq = HttpOpenRequestA(hConn, "POST", url.c_str(), nullptr, nullptr,
                                      nullptr, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 0);
    if (!hReq) { InternetCloseHandle(hConn); InternetCloseHandle(hInet); return false; }

    const char* headers = "Content-Type: application/x-www-form-urlencoded\r\n";
    bool ok = HttpSendRequestA(hReq, headers, (DWORD)strlen(headers),
                               (void*)postData.data(), (DWORD)postData.size()) != 0;

    if (ok) {
        // Check response
        std::string resp;
        char buf[4096];
        DWORD bytesRead;
        while (InternetReadFile(hReq, buf, sizeof(buf), &bytesRead) && bytesRead > 0) {
            resp.append(buf, bytesRead);
        }
        ok = resp.find("\"ok\":true") != std::string::npos;
        if (!ok) {
            Log::Error(L"SendLogToTelegram: API returned error");
        }
    }

    InternetCloseHandle(hReq);
    InternetCloseHandle(hConn);
    InternetCloseHandle(hInet);

    if (ok) Log::Info(L"Log sent to Telegram successfully");
    return ok;
}
