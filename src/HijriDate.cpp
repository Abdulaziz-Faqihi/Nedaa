#include "HijriDate.h"
#include <windows.h>
#include <sstream>
#include <string>

// English Hijri month names
static const wchar_t* s_hijriMonthsEN[] = {
    L"Muharram", L"Safar", L"Rabi al-Awwal", L"Rabi al-Thani",
    L"Jumada al-Ula", L"Jumada al-Thani", L"Rajab", L"Shaban",
    L"Ramadan", L"Shawwal", L"Dhul Qidah", L"Dhul Hijjah"
};

std::wstring GetHijriDateString(Language lang) {
    SYSTEMTIME st;
    GetLocalTime(&st);

    if (lang == Language::AR) {
        // Arabic: use ar-SA directly, it defaults to Umm al-Qura
        wchar_t hijriDate[128] = {};
        int ret = GetDateFormatEx(L"ar-SA", DATE_LONGDATE, &st, nullptr, hijriDate, 128, nullptr);
        if (ret > 0 && hijriDate[0] != 0) {
            return std::wstring(hijriDate);
        }
        return L"";
    }

    // English: extract Hijri day/month/year numbers using ar-SA, then format in English
    wchar_t dayBuf[8] = {};
    wchar_t monthBuf[8] = {};
    wchar_t yearBuf[16] = {};

    GetDateFormatEx(L"ar-SA", 0, &st, L"d", dayBuf, 8, nullptr);
    GetDateFormatEx(L"ar-SA", 0, &st, L"M", monthBuf, 8, nullptr);
    GetDateFormatEx(L"ar-SA", 0, &st, L"yyyy", yearBuf, 16, nullptr);

    // Convert Arabic-Indic digits to Western digits
    auto convertDigits = [](wchar_t* buf) {
        for (wchar_t* p = buf; *p; ++p) {
            // Arabic-Indic digits U+0660..U+0669
            if (*p >= 0x0660 && *p <= 0x0669)
                *p = L'0' + (*p - 0x0660);
            // Extended Arabic-Indic digits U+06F0..U+06F9
            if (*p >= 0x06F0 && *p <= 0x06F9)
                *p = L'0' + (*p - 0x06F0);
        }
    };

    convertDigits(dayBuf);
    convertDigits(monthBuf);
    convertDigits(yearBuf);

    int monthNum = _wtoi(monthBuf);
    const wchar_t* monthName = L"";
    if (monthNum >= 1 && monthNum <= 12) {
        monthName = s_hijriMonthsEN[monthNum - 1];
    }

    std::wostringstream oss;
    oss << dayBuf << L" " << monthName << L" " << yearBuf << L" AH";
    return oss.str();
}
