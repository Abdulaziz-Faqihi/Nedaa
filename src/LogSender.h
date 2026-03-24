#pragma once
#include <string>

namespace LogSender {
    // Send the current log file to the support Telegram bot with a note
    bool SendLogToTelegram(const std::wstring& userNote = L"");

    // Send a text-only message (no log file attached)
    bool SendTextToTelegram(const std::wstring& text);
}
