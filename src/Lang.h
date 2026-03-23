#pragma once
#include <string>

enum class Language { AR = 0, EN = 1 };

namespace Lang {
    // UI
    const wchar_t* SettingsTitle(Language lang);
    const wchar_t* CountryLabel(Language lang);
    const wchar_t* RegionLabel(Language lang);
    const wchar_t* CityLabel(Language lang);
    const wchar_t* HotkeyLabel(Language lang);
    const wchar_t* KeyLabel(Language lang);
    const wchar_t* SaveButton(Language lang);
    const wchar_t* LanguageLabel(Language lang);

    // Tray menu
    const wchar_t* MenuSettings(Language lang);
    const wchar_t* MenuExit(Language lang);
    const wchar_t* TrayTooltip(Language lang);

    // Prayer names
    const wchar_t* PrayerName(int index, Language lang);

    // Speech output
    const wchar_t* NextPrayer(Language lang);
    const wchar_t* Remaining(Language lang);
    const wchar_t* ElapsedSince(Language lang);
    const wchar_t* Hour(Language lang);
    const wchar_t* Minute(Language lang);
    const wchar_t* And(Language lang);

    // Iqama
    const wchar_t* IqamaIn(Language lang);

    // Download
    const wchar_t* DownloadingData(Language lang);
    const wchar_t* DownloadFailed(Language lang);

    // Update
    const wchar_t* UpdateAvailable(Language lang);
    const wchar_t* DownloadingUpdate(Language lang);
    const wchar_t* MenuCheckUpdate(Language lang);
    const wchar_t* CheckUpdateButton(Language lang);
    const wchar_t* NoUpdateAvailable(Language lang);

    // Calculation method
    const wchar_t* CalcMethodLabel(Language lang);
    const wchar_t* CalcMethodName(int index, Language lang);
    const wchar_t* CalcMethodAuto(Language lang);

    // Log sending
    const wchar_t* SendLogButton(Language lang);
    const wchar_t* SendingLog(Language lang);
    const wchar_t* LogSentSuccess(Language lang);
    const wchar_t* LogSentFailed(Language lang);
}
