#include "TrayIcon.h"
#include "SettingsFrame.h"
#include "PrayerTimes.h"
#include "HijriDate.h"
#include "Speech.h"
#include "Config.h"
#include "Cities.h"
#include "Lang.h"
#include <wx/wx.h>
#include <ctime>
#include <sstream>

static const int HOTKEY_ID = 1;

// Iqama offset in minutes per prayer (0 = no iqama, e.g. sunrise)
static int GetIqamaOffsetMinutes(int prayer) {
    switch (prayer) {
        case FAJR:    return 25;
        case DHUHR:   return 20;
        case ASR:     return 20;
        case MAGHRIB: return 10;
        case ISHA:    return 20;
        default:      return 0;
    }
}

NidaaTrayIcon::NidaaTrayIcon() {
    UpdateTooltip();
}

void NidaaTrayIcon::UpdateTooltip() {
    AppConfig config;
    ConfigLoad(config);
    Language lang = static_cast<Language>(config.language);
    SetIcon(wxICON(wxICON_INFORMATION), Lang::TrayTooltip(lang));
}

NidaaTrayIcon::~NidaaTrayIcon() {
    RemoveIcon();
}

wxMenu* NidaaTrayIcon::CreatePopupMenu() {
    AppConfig config;
    ConfigLoad(config);
    Language lang = static_cast<Language>(config.language);

    auto* menu = new wxMenu;
    menu->Append(ID_SETTINGS, Lang::MenuSettings(lang));
    menu->AppendSeparator();
    menu->Append(ID_EXIT, Lang::MenuExit(lang));

    menu->Bind(wxEVT_MENU, &NidaaTrayIcon::OnSettings, this, ID_SETTINGS);
    menu->Bind(wxEVT_MENU, &NidaaTrayIcon::OnExit, this, ID_EXIT);
    return menu;
}

void NidaaTrayIcon::OnSettings(wxCommandEvent&) {
    if (m_settingsFrame) {
        m_settingsFrame->Show(true);
        m_settingsFrame->Raise();
    }
}

void NidaaTrayIcon::OnExit(wxCommandEvent&) {
    RemoveIcon();
    if (m_settingsFrame) {
        m_settingsFrame->Destroy();
        m_settingsFrame = nullptr;
    }
    wxTheApp->ExitMainLoop();
}

void NidaaTrayIcon::RegisterGlobalHotkey(void* hwnd, int modifiers, int vk) {
    ::RegisterHotKey(static_cast<HWND>(hwnd), HOTKEY_ID, modifiers, vk);
}

void NidaaTrayIcon::UnregisterGlobalHotkey(void* hwnd) {
    ::UnregisterHotKey(static_cast<HWND>(hwnd), HOTKEY_ID);
}

static void AppendDuration(std::wostringstream& oss, int totalMinutes, Language lang) {
    int hours = totalMinutes / 60;
    int mins = totalMinutes % 60;
    if (hours > 0) {
        oss << hours << Lang::Hour(lang);
        if (hours > 1 && lang == Language::EN) oss << L"s";
        if (mins > 0) {
            oss << Lang::And(lang) << mins << Lang::Minute(lang);
            if (mins > 1 && lang == Language::EN) oss << L"s";
        }
    } else {
        oss << mins << Lang::Minute(lang);
        if (mins > 1 && lang == Language::EN) oss << L"s";
    }
}

void NidaaTrayIcon::OnHotkeyPressed() {
    AppConfig config;
    ConfigLoad(config);
    Language lang = static_cast<Language>(config.language);

    auto& db = CitiesDatabase::Instance();
    const CountryInfo* country = db.FindCountryByIso2(config.countryIso2);

    double latitude = 21.4225;   // Makkah fallback
    double longitude = 39.8262;
    double timezone = 3.0;
    CalcMethodId calcMethod = CalcMethodId::UMM_AL_QURA;

    if (country) {
        timezone = country->timezoneHours;
        calcMethod = country->calcMethod;

        if (country->id == CitiesDatabase::SAUDI_COUNTRY_ID && config.saudiCityId > 0) {
            const CityInfo* city = db.FindSaudiCityById(config.saudiCityId);
            if (city) {
                latitude = city->latitude;
                longitude = city->longitude;
            }
        } else if (config.stateId > 0) {
            const StateInfo* state = db.FindStateById(config.stateId);
            if (state) {
                latitude = state->latitude;
                longitude = state->longitude;
            }
        }
    }

    time_t now = time(nullptr);
    struct tm lt;
    localtime_s(&lt, &now);

    PrayerTimesCalculator calc;
    auto schedule = calc.Calculate(
        lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday,
        latitude, longitude, timezone, calcMethod);

    double currentHours = lt.tm_hour + lt.tm_min / 60.0 + lt.tm_sec / 3600.0;
    auto info = PrayerTimesCalculator::FindNextPrayer(schedule, currentHours);

    std::wstring hijri = GetHijriDateString(lang);

    std::wostringstream oss;

    // Hijri date
    oss << hijri << L". ";

    // Next prayer
    oss << Lang::NextPrayer(lang);
    oss << PrayerTimesCalculator::GetPrayerName(info.nextPrayer, lang);
    oss << L" " << PrayerTimesCalculator::FormatTime(info.nextTime) << L". ";

    // Remaining
    oss << Lang::Remaining(lang);
    AppendDuration(oss, info.remainingMinutes, lang);
    oss << L". ";

    // Iqama countdown (only during iqama window)
    if (info.prevPrayer >= 0) {
        int iqamaOffset = GetIqamaOffsetMinutes(info.prevPrayer);
        if (iqamaOffset > 0 && info.elapsedMinutes < iqamaOffset) {
            int iqamaRemaining = iqamaOffset - info.elapsedMinutes;
            oss << Lang::IqamaIn(lang);
            AppendDuration(oss, iqamaRemaining, lang);
            oss << L".";
        }
    }

    SpeechSay(oss.str());
}
