#pragma once
#include <string>

namespace LogSender {
    // Send the current log file to the support Telegram bot
    // Returns true on success
    bool SendLogToTelegram(const std::wstring& userNote = L"");
}
