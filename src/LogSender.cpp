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

static std::string DiscoverChatId(const std::string& token) {
    std::string chatId;

    HINTERNET hInet = InternetOpenA("Nidaa", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    if (!hInet) return "";

    HINTERNET hConn = InternetConnectA(hInet, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT,
                                       nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConn) { InternetCloseHandle(hInet); return ""; }

    std::string getUpdatesUrl = "/bot" + token + "/getUpdates?limit=1";
    HINTERNET hReq = HttpOpenRequestA(hConn, "GET", getUpdatesUrl.c_str(), nullptr, nullptr,
                                      nullptr, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 0);
    if (!hReq) { InternetCloseHandle(hConn); InternetCloseHandle(hInet); return ""; }

    if (HttpSendRequestA(hReq, nullptr, 0, nullptr, 0)) {
        std::string resp;
        char buf[4096];
        DWORD bytesRead;
        while (InternetReadFile(hReq, buf, sizeof(buf), &bytesRead) && bytesRead > 0) {
            resp.append(buf, bytesRead);
        }
        auto chatIdPos = resp.find("\"chat\":{\"id\":");
        if (chatIdPos != std::string::npos) {
            chatIdPos += 13;
            auto end = resp.find_first_of(",}", chatIdPos);
            if (end != std::string::npos) {
                chatId = resp.substr(chatIdPos, end - chatIdPos);
            }
        }
    }
    InternetCloseHandle(hReq);
    InternetCloseHandle(hConn);
    InternetCloseHandle(hInet);
    return chatId;
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

    std::string token = TELEGRAM_BOT_TOKEN;
    std::string chatId = TELEGRAM_CHAT_ID;

    if (chatId.empty()) {
        chatId = DiscoverChatId(token);
        if (chatId.empty()) {
            Log::Error(L"SendLogToTelegram: could not determine chat_id. Send /start to the bot first.");
            return false;
        }
    }

    // Build multipart/form-data to send log as a file via sendDocument
    std::string boundary = "----NidaaLogBoundary9f8e7d6c";
    std::string body;

    // chat_id field
    body += "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n";
    body += chatId + "\r\n";

    // caption field (short description)
    std::string caption = "Nidaa v" NIDAA_VERSION " log";
    if (!userNote.empty()) {
        caption += " - " + WideToUtf8(userNote);
    }
    if (caption.size() > 1024) caption = caption.substr(0, 1024);
    body += "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"caption\"\r\n\r\n";
    body += caption + "\r\n";

    // document field (the log file)
    body += "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"document\"; filename=\"nidaa.log\"\r\n";
    body += "Content-Type: text/plain; charset=utf-8\r\n\r\n";
    body += logContent + "\r\n";

    // End boundary
    body += "--" + boundary + "--\r\n";

    // Send via WinINet
    std::string url = "/bot" + token + "/sendDocument";
    std::string contentType = "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n";

    HINTERNET hInet = InternetOpenA("Nidaa", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    if (!hInet) return false;

    HINTERNET hConn = InternetConnectA(hInet, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT,
                                       nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConn) { InternetCloseHandle(hInet); return false; }

    HINTERNET hReq = HttpOpenRequestA(hConn, "POST", url.c_str(), nullptr, nullptr,
                                      nullptr, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 0);
    if (!hReq) { InternetCloseHandle(hConn); InternetCloseHandle(hInet); return false; }

    bool ok = HttpSendRequestA(hReq, contentType.c_str(), (DWORD)contentType.size(),
                               (void*)body.data(), (DWORD)body.size()) != 0;

    if (ok) {
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
