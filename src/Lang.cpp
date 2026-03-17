#include "Lang.h"
#include "PrayerTimes.h"

namespace Lang {

const wchar_t* SettingsTitle(Language lang) {
    return lang == Language::AR
        ? L"\x0625\x0639\x062F\x0627\x062F\x0627\x062A \x0646\x062F\x0627\x0621" // إعدادات نداء
        : L"Nidaa Settings";
}

const wchar_t* CountryLabel(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x062F\x0648\x0644\x0629:" // الدولة:
        : L"Country:";
}

const wchar_t* CityLabel(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x0645\x062F\x064A\x0646\x0629:" // المدينة:
        : L"City:";
}

const wchar_t* RegionLabel(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x0645\x0646\x0637\x0642\x0629:" // المنطقة:
        : L"Region:";
}

const wchar_t* HotkeyLabel(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x0627\x062E\x062A\x0635\x0627\x0631:" // الاختصار:
        : L"Hotkey:";
}

const wchar_t* KeyLabel(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x0645\x0641\x062A\x0627\x062D:" // المفتاح:
        : L"Key:";
}

const wchar_t* SaveButton(Language lang) {
    return lang == Language::AR
        ? L"\x062D\x0641\x0638" // حفظ
        : L"Save";
}

const wchar_t* LanguageLabel(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x0644\x063A\x0629:" // اللغة:
        : L"Language:";
}

const wchar_t* MenuSettings(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x0625\x0639\x062F\x0627\x062F\x0627\x062A" // الإعدادات
        : L"Settings";
}

const wchar_t* MenuExit(Language lang) {
    return lang == Language::AR
        ? L"\x062E\x0631\x0648\x062C" // خروج
        : L"Exit";
}

const wchar_t* TrayTooltip(Language lang) {
    return lang == Language::AR
        ? L"\x0646\x062F\x0627\x0621" // نداء
        : L"Nidaa";
}

const wchar_t* PrayerName(int index, Language lang) {
    if (lang == Language::AR) {
        switch (index) {
            case FAJR:    return L"\x0627\x0644\x0641\x062C\x0631";      // الفجر
            case SUNRISE: return L"\x0627\x0644\x0634\x0631\x0648\x0642"; // الشروق
            case DHUHR:   return L"\x0627\x0644\x0638\x0647\x0631";       // الظهر
            case ASR:     return L"\x0627\x0644\x0639\x0635\x0631";       // العصر
            case MAGHRIB: return L"\x0627\x0644\x0645\x063A\x0631\x0628"; // المغرب
            case ISHA:    return L"\x0627\x0644\x0639\x0634\x0627\x0621"; // العشاء
            default:      return L"";
        }
    } else {
        switch (index) {
            case FAJR:    return L"Fajr";
            case SUNRISE: return L"Sunrise";
            case DHUHR:   return L"Dhuhr";
            case ASR:     return L"Asr";
            case MAGHRIB: return L"Maghrib";
            case ISHA:    return L"Isha";
            default:      return L"";
        }
    }
}

const wchar_t* NextPrayer(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x0635\x0644\x0627\x0629 \x0627\x0644\x0642\x0627\x062F\x0645\x0629: " // الصلاة القادمة:
        : L"Next prayer: ";
}

const wchar_t* Remaining(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x0645\x062A\x0628\x0642\x064A: " // المتبقي:
        : L"Remaining: ";
}

const wchar_t* ElapsedSince(Language lang) {
    return lang == Language::AR
        ? L"\x0645\x0636\x0649 \x0639\x0644\x0649 " // مضى على
        : L"Since ";
}

const wchar_t* Hour(Language lang) {
    return lang == Language::AR
        ? L" \x0633\x0627\x0639\x0629" // ساعة
        : L" hour";
}

const wchar_t* Minute(Language lang) {
    return lang == Language::AR
        ? L" \x062F\x0642\x064A\x0642\x0629" // دقيقة
        : L" minute";
}

const wchar_t* And(Language lang) {
    return lang == Language::AR
        ? L" \x0648 " // و
        : L" and ";
}

const wchar_t* DownloadingData(Language lang) {
    return lang == Language::AR
        ? L"\x062C\x0627\x0631\x064A \x062A\x062D\x0645\x064A\x0644 \x0627\x0644\x0628\x064A\x0627\x0646\x0627\x062A..." // جاري تحميل البيانات...
        : L"Downloading data...";
}

const wchar_t* DownloadFailed(Language lang) {
    return lang == Language::AR
        ? L"\x0641\x0634\x0644 \x062A\x062D\x0645\x064A\x0644 \x0627\x0644\x0628\x064A\x0627\x0646\x0627\x062A" // فشل تحميل البيانات
        : L"Failed to download data";
}

} // namespace Lang
