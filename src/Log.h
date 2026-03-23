#pragma once
#include <string>

namespace Log {
    void Init();
    void Info(const std::wstring& msg);
    void Error(const std::wstring& msg);
    std::wstring GetLogFilePath();
    std::string ReadLogFileUtf8();
}
