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

const wchar_t* IqamaIn(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x0625\x0642\x0627\x0645\x0629 \x0628\x0639\x062F: " // الإقامة بعد:
        : L"Iqama in: ";
}

const wchar_t* DownloadingUpdate(Language lang) {
    return lang == Language::AR
        ? L"\x062C\x0627\x0631\x064A \x062A\x062D\x0645\x064A\x0644 \x0627\x0644\x062A\x062D\x062F\x064A\x062B..." // جاري تحميل التحديث...
        : L"Downloading update...";
}

const wchar_t* UpdateAvailable(Language lang) {
    return lang == Language::AR
        ? L"\x062C\x0627\x0631\x064A \x062A\x062B\x0628\x064A\x062A \x0627\x0644\x062A\x062D\x062F\x064A\x062B" // جاري تثبيت التحديث
        : L"Installing update...";
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

const wchar_t* MenuCheckUpdate(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x062A\x062D\x0642\x0642 \x0645\x0646 \x0627\x0644\x062A\x062D\x062F\x064A\x062B\x0627\x062A" // التحقق من التحديثات
        : L"Check for updates";
}

const wchar_t* CheckUpdateButton(Language lang) {
    return lang == Language::AR
        ? L"\x0627\x0644\x062A\x062D\x0642\x0642 \x0645\x0646 \x0627\x0644\x062A\x062D\x062F\x064A\x062B\x0627\x062A" // التحقق من التحديثات
        : L"Check for updates";
}

const wchar_t* NoUpdateAvailable(Language lang) {
    return lang == Language::AR
        ? L"\x0644\x0627 \x062A\x0648\x062C\x062F \x062A\x062D\x062F\x064A\x062B\x0627\x062A \x062C\x062F\x064A\x062F\x0629" // لا توجد تحديثات جديدة
        : L"No updates available";
}

const wchar_t* CalcMethodLabel(Language lang) {
    return lang == Language::AR
        ? L"\x0637\x0631\x064A\x0642\x0629 \x0627\x0644\x062D\x0633\x0627\x0628:" // طريقة الحساب:
        : L"Calculation method:";
}

const wchar_t* CalcMethodAuto(Language lang) {
    return lang == Language::AR
        ? L"\x062A\x0644\x0642\x0627\x0626\x064A (\x062D\x0633\x0628 \x0627\x0644\x062F\x0648\x0644\x0629)" // تلقائي (حسب الدولة)
        : L"Auto (based on country)";
}

const wchar_t* CalcMethodName(int index, Language lang) {
    if (lang == Language::AR) {
        switch (index) {
            case 0: return L"\x0623\x0645 \x0627\x0644\x0642\x0631\x0649";                           // أم القرى
            case 1: return L"\x0631\x0627\x0628\x0637\x0629 \x0627\x0644\x0639\x0627\x0644\x0645 \x0627\x0644\x0625\x0633\x0644\x0627\x0645\x064A"; // رابطة العالم الإسلامي
            case 2: return L"\x0627\x0644\x0647\x064A\x0626\x0629 \x0627\x0644\x0645\x0635\x0631\x064A\x0629"; // الهيئة المصرية
            case 3: return L"\x062C\x0627\x0645\x0639\x0629 \x0643\x0631\x0627\x062A\x0634\x064A";   // جامعة كراتشي
            case 4: return L"\x0627\x0644\x0645\x0646\x0638\x0645\x0629 \x0627\x0644\x0625\x0633\x0644\x0627\x0645\x064A\x0629 \x0644\x0623\x0645\x0631\x064A\x0643\x0627 \x0627\x0644\x0634\x0645\x0627\x0644\x064A\x0629"; // ISNA
            case 5: return L"\x0627\x0644\x062F\x064A\x0627\x0646\x0629 \x0627\x0644\x062A\x0631\x0643\x064A\x0629"; // الديانة التركية
            case 6: return L"\x0645\x0639\x0647\x062F \x0637\x0647\x0631\x0627\x0646";               // معهد طهران
            case 7: return L"\x0633\x0646\x063A\x0627\x0641\x0648\x0631\x0629/\x0645\x0627\x0644\x064A\x0632\x064A\x0627"; // سنغافورة/ماليزيا
            default: return L"";
        }
    } else {
        switch (index) {
            case 0: return L"Umm Al-Qura";
            case 1: return L"Muslim World League";
            case 2: return L"Egyptian General Authority";
            case 3: return L"University of Karachi";
            case 4: return L"ISNA (North America)";
            case 5: return L"Diyanet (Turkey)";
            case 6: return L"Institute of Geophysics, Tehran";
            case 7: return L"Singapore/Malaysia";
            default: return L"";
        }
    }
}

const wchar_t* SendLogButton(Language lang) {
    return lang == Language::AR
        ? L"\x0625\x0631\x0633\x0627\x0644 \x0627\x0644\x0633\x062C\x0644\x0627\x062A \x0644\x0644\x062F\x0639\x0645" // إرسال السجلات للدعم
        : L"Send logs to support";
}

const wchar_t* SendingLog(Language lang) {
    return lang == Language::AR
        ? L"\x062C\x0627\x0631\x064A \x0625\x0631\x0633\x0627\x0644 \x0627\x0644\x0633\x062C\x0644\x0627\x062A" // جاري إرسال السجلات
        : L"Sending logs...";
}

const wchar_t* LogSentSuccess(Language lang) {
    return lang == Language::AR
        ? L"\x062A\x0645 \x0625\x0631\x0633\x0627\x0644 \x0627\x0644\x0633\x062C\x0644\x0627\x062A \x0628\x0646\x062C\x0627\x062D" // تم إرسال السجلات بنجاح
        : L"Logs sent successfully";
}

const wchar_t* LogSentFailed(Language lang) {
    return lang == Language::AR
        ? L"\x0641\x0634\x0644 \x0625\x0631\x0633\x0627\x0644 \x0627\x0644\x0633\x062C\x0644\x0627\x062A. \x0623\x0631\x0633\x0644 /start \x0644\x0644\x0628\x0648\x062A \x0623\x0648\x0644\x0627\x064B" // فشل إرسال السجلات. أرسل /start للبوت أولاً
        : L"Failed to send logs. Send /start to the bot first.";
}

} // namespace Lang
