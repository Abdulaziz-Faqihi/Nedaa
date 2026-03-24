#include "SettingsFrame.h"
#include "TrayIcon.h"
#include "Config.h"
#include "Cities.h"
#include "CalcMethod.h"
#include "Updater.h"
#include "Speech.h"
#include "Log.h"
#include "SupportDialog.h"
#include <wx/sizer.h>
#include <wx/msw/wrapwin.h>
#include <algorithm>
#include <numeric>

SettingsFrame::SettingsFrame(NidaaTrayIcon* tray)
    : wxFrame(nullptr, wxID_ANY,
              Lang::SettingsTitle(Language::AR),
              wxDefaultPosition, wxSize(450, 600),
              wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
    , m_tray(tray)
{
    auto* panel = new wxPanel(this);
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Language selection
    {
        m_lblLang = new wxStaticText(panel, wxID_ANY, Lang::LanguageLabel(Language::AR));
        mainSizer->Add(m_lblLang, 0, wxALL, 5);

        m_langChoice = new wxChoice(panel, ID_LANG);
        m_langChoice->SetName(L"Language");
        m_langChoice->Append(L"\x0627\x0644\x0639\x0631\x0628\x064A\x0629"); // العربية
        m_langChoice->Append(L"English");
        m_langChoice->SetSelection(0);
        mainSizer->Add(m_langChoice, 0, wxEXPAND | wxALL, 5);
        m_langChoice->Bind(wxEVT_CHOICE, &SettingsFrame::OnLanguageChanged, this, ID_LANG);
    }

    // Country selection
    {
        m_lblCountry = new wxStaticText(panel, wxID_ANY, Lang::CountryLabel(Language::AR));
        mainSizer->Add(m_lblCountry, 0, wxALL, 5);

        m_countryChoice = new wxChoice(panel, ID_COUNTRY);
        m_countryChoice->SetName(Lang::CountryLabel(Language::AR));
        mainSizer->Add(m_countryChoice, 0, wxEXPAND | wxALL, 5);
        m_countryChoice->Bind(wxEVT_CHOICE, &SettingsFrame::OnCountryChanged, this, ID_COUNTRY);
    }

    // Region selection
    {
        m_lblRegion = new wxStaticText(panel, wxID_ANY, Lang::RegionLabel(Language::AR));
        mainSizer->Add(m_lblRegion, 0, wxALL, 5);

        m_regionChoice = new wxChoice(panel, ID_REGION);
        m_regionChoice->SetName(Lang::RegionLabel(Language::AR));
        mainSizer->Add(m_regionChoice, 0, wxEXPAND | wxALL, 5);
        m_regionChoice->Bind(wxEVT_CHOICE, &SettingsFrame::OnRegionChanged, this, ID_REGION);
    }

    // City selection
    {
        m_lblCity = new wxStaticText(panel, wxID_ANY, Lang::CityLabel(Language::AR));
        mainSizer->Add(m_lblCity, 0, wxALL, 5);

        m_cityChoice = new wxChoice(panel, wxID_ANY);
        m_cityChoice->SetName(Lang::CityLabel(Language::AR));
        mainSizer->Add(m_cityChoice, 0, wxEXPAND | wxALL, 5);
    }

    // Calculation method
    {
        m_lblCalcMethod = new wxStaticText(panel, wxID_ANY, Lang::CalcMethodLabel(Language::AR));
        mainSizer->Add(m_lblCalcMethod, 0, wxALL, 5);

        m_calcMethodChoice = new wxChoice(panel, wxID_ANY);
        m_calcMethodChoice->SetName(Lang::CalcMethodLabel(Language::AR));
        mainSizer->Add(m_calcMethodChoice, 0, wxEXPAND | wxALL, 5);
    }

    // Hotkey settings
    {
        m_lblHotkey = new wxStaticText(panel, wxID_ANY, Lang::HotkeyLabel(Language::AR));
        mainSizer->Add(m_lblHotkey, 0, wxALL, 5);

        auto* modSizer = new wxBoxSizer(wxHORIZONTAL);
        m_chkCtrl = new wxCheckBox(panel, wxID_ANY, L"Ctrl");
        m_chkAlt = new wxCheckBox(panel, wxID_ANY, L"Alt");
        m_chkShift = new wxCheckBox(panel, wxID_ANY, L"Shift");
        modSizer->Add(m_chkCtrl, 0, wxALL, 5);
        modSizer->Add(m_chkAlt, 0, wxALL, 5);
        modSizer->Add(m_chkShift, 0, wxALL, 5);
        mainSizer->Add(modSizer, 0, wxEXPAND);

        m_lblKey = new wxStaticText(panel, wxID_ANY, Lang::KeyLabel(Language::AR));
        mainSizer->Add(m_lblKey, 0, wxALL, 5);

        m_keyText = new wxTextCtrl(panel, wxID_ANY, L"P");
        m_keyText->SetName(Lang::KeyLabel(Language::AR));
        m_keyText->SetMaxLength(1);
        mainSizer->Add(m_keyText, 0, wxEXPAND | wxALL, 5);
    }

    // Save button
    {
        m_saveBtn = new wxButton(panel, ID_SAVE, Lang::SaveButton(Language::AR));
        mainSizer->Add(m_saveBtn, 0, wxALIGN_CENTER | wxALL, 10);
        m_saveBtn->Bind(wxEVT_BUTTON, &SettingsFrame::OnSave, this);
    }

    // Check for updates button
    {
        m_updateBtn = new wxButton(panel, ID_CHECK_UPDATE, Lang::CheckUpdateButton(Language::AR));
        mainSizer->Add(m_updateBtn, 0, wxALIGN_CENTER | wxALL, 5);
        m_updateBtn->Bind(wxEVT_BUTTON, &SettingsFrame::OnCheckUpdate, this);
    }

    // Support button
    {
        m_supportBtn = new wxButton(panel, ID_SUPPORT, Lang::SupportButton(Language::AR));
        mainSizer->Add(m_supportBtn, 0, wxALIGN_CENTER | wxALL, 5);
        m_supportBtn->Bind(wxEVT_BUTTON, &SettingsFrame::OnSupport, this);
    }

    panel->SetSizer(mainSizer);
    Bind(wxEVT_CLOSE_WINDOW, &SettingsFrame::OnClose, this);

    LoadCurrentSettings();
}

void SettingsFrame::PopulateCountries() {
    m_countryChoice->Freeze();
    m_countryChoice->Clear();
    m_countryIds.clear();

    const auto& countries = CitiesDatabase::Instance().GetCountries();

    // Build indices sorted alphabetically by current display language
    std::vector<size_t> order(countries.size());
    for (size_t i = 0; i < countries.size(); ++i) order[i] = i;
    std::sort(order.begin(), order.end(), [&](size_t a, size_t b) {
        if (m_currentLang == Language::AR)
            return countries[a].nameAr < countries[b].nameAr;
        else
            return countries[a].nameEn < countries[b].nameEn;
    });

    for (size_t idx : order) {
        const auto& c = countries[idx];
        if (m_currentLang == Language::AR)
            m_countryChoice->Append(c.nameAr);
        else
            m_countryChoice->Append(c.nameEn);
        m_countryIds.push_back(c.id);
    }
    if (!countries.empty())
        m_countryChoice->SetSelection(0);

    m_countryChoice->Thaw();
}

void SettingsFrame::PopulateRegions() {
    m_regionChoice->Freeze();
    m_regionChoice->Clear();
    m_stateIds.clear();
    m_saudiRegionIds.clear();

    if (IsSaudiSelected()) {
        const auto& regions = CitiesDatabase::Instance().GetSaudiRegions();
        for (const auto& r : regions) {
            if (m_currentLang == Language::AR)
                m_regionChoice->Append(r.nameAr);
            else
                m_regionChoice->Append(r.nameEn);
            m_saudiRegionIds.push_back(r.regionId);
        }
    } else {
        int countryId = GetSelectedCountryId();
        if (countryId > 0) {
            auto states = CitiesDatabase::Instance().GetStatesByCountry(countryId);
            for (const auto* s : states) {
                if (m_currentLang == Language::AR)
                    m_regionChoice->Append(s->nameAr);
                else
                    m_regionChoice->Append(s->nameEn);
                m_stateIds.push_back(s->id);
            }
        }
    }

    if (m_regionChoice->GetCount() > 0)
        m_regionChoice->SetSelection(0);

    Log::Info(L"PopulateRegions: " + std::to_wstring(m_regionChoice->GetCount()) + L" regions loaded"
              + (IsSaudiSelected() ? L" (Saudi)" : L" (countryId=" + std::to_wstring(GetSelectedCountryId()) + L")"));

    m_regionChoice->Thaw();
}

void SettingsFrame::PopulateCities() {
    m_cityChoice->Freeze();
    m_cityChoice->Clear();
    m_saudiCityIds.clear();

    if (IsSaudiSelected()) {
        int regionSel = m_regionChoice->GetSelection();
        if (regionSel >= 0 && regionSel < static_cast<int>(m_saudiRegionIds.size())) {
            int regionId = m_saudiRegionIds[regionSel];
            auto cities = CitiesDatabase::Instance().GetSaudiCitiesByRegion(regionId);
            for (const auto* c : cities) {
                if (m_currentLang == Language::AR)
                    m_cityChoice->Append(c->nameAr);
                else
                    m_cityChoice->Append(c->nameEn);
                m_saudiCityIds.push_back(c->cityId);
            }
        }
        m_cityChoice->Enable(true);
        if (m_cityChoice->GetCount() > 0)
            m_cityChoice->SetSelection(0);
    } else {
        m_cityChoice->Enable(false);
    }

    Log::Info(L"PopulateCities: " + std::to_wstring(m_cityChoice->GetCount()) + L" cities loaded");

    m_cityChoice->Thaw();
}

void SettingsFrame::PopulateCalcMethods() {
    m_calcMethodChoice->Freeze();
    m_calcMethodChoice->Clear();

    // First item: Auto (based on country)
    m_calcMethodChoice->Append(Lang::CalcMethodAuto(m_currentLang));

    // Add all 8 calculation methods
    for (int i = 0; i < 8; ++i) {
        m_calcMethodChoice->Append(Lang::CalcMethodName(i, m_currentLang));
    }

    m_calcMethodChoice->SetSelection(0);
    m_calcMethodChoice->Thaw();
}

void SettingsFrame::OnCheckUpdate(wxCommandEvent&) {
    std::wstring installerPath;
    if (Updater::CheckForUpdate(installerPath)) {
        SpeechSay(Lang::UpdateAvailable(m_currentLang));
        ShellExecuteW(nullptr, L"open", installerPath.c_str(),
                      L"/SILENT", nullptr, SW_SHOWNORMAL);
        wxTheApp->ExitMainLoop();
    } else {
        SpeechSay(Lang::NoUpdateAvailable(m_currentLang));
    }
}

void SettingsFrame::OnSupport(wxCommandEvent&) {
    SupportDialog dlg(this, m_currentLang);
    dlg.ShowModal();
}

void SettingsFrame::RefreshUI() {
    SetTitle(Lang::SettingsTitle(m_currentLang));
    m_lblLang->SetLabel(Lang::LanguageLabel(m_currentLang));
    m_lblCountry->SetLabel(Lang::CountryLabel(m_currentLang));
    m_lblRegion->SetLabel(Lang::RegionLabel(m_currentLang));
    m_lblCity->SetLabel(Lang::CityLabel(m_currentLang));
    m_lblCalcMethod->SetLabel(Lang::CalcMethodLabel(m_currentLang));
    m_lblHotkey->SetLabel(Lang::HotkeyLabel(m_currentLang));
    m_lblKey->SetLabel(Lang::KeyLabel(m_currentLang));
    m_saveBtn->SetLabel(Lang::SaveButton(m_currentLang));
    m_updateBtn->SetLabel(Lang::CheckUpdateButton(m_currentLang));
    m_supportBtn->SetLabel(Lang::SupportButton(m_currentLang));
    m_countryChoice->SetName(Lang::CountryLabel(m_currentLang));
    m_regionChoice->SetName(Lang::RegionLabel(m_currentLang));
    m_cityChoice->SetName(Lang::CityLabel(m_currentLang));
    m_calcMethodChoice->SetName(Lang::CalcMethodLabel(m_currentLang));
    m_keyText->SetName(Lang::KeyLabel(m_currentLang));

    // Remember current selections
    int savedCountrySel = m_countryChoice->GetSelection();
    int savedRegionSel = m_regionChoice->GetSelection();
    int savedCitySel = m_cityChoice->GetSelection();
    int savedCalcSel = m_calcMethodChoice->GetSelection();

    PopulateCountries();

    if (savedCountrySel >= 0 && savedCountrySel < static_cast<int>(m_countryChoice->GetCount()))
        m_countryChoice->SetSelection(savedCountrySel);

    PopulateRegions();

    if (savedRegionSel >= 0 && savedRegionSel < static_cast<int>(m_regionChoice->GetCount()))
        m_regionChoice->SetSelection(savedRegionSel);

    PopulateCities();

    if (savedCitySel >= 0 && savedCitySel < static_cast<int>(m_cityChoice->GetCount()))
        m_cityChoice->SetSelection(savedCitySel);

    PopulateCalcMethods();
    if (savedCalcSel >= 0 && savedCalcSel < static_cast<int>(m_calcMethodChoice->GetCount()))
        m_calcMethodChoice->SetSelection(savedCalcSel);

    // Set layout direction
    if (m_currentLang == Language::AR) {
        SetLayoutDirection(wxLayout_RightToLeft);
    } else {
        SetLayoutDirection(wxLayout_LeftToRight);
    }

    Layout();
}

void SettingsFrame::OnLanguageChanged(wxCommandEvent&) {
    int sel = m_langChoice->GetSelection();
    m_currentLang = (sel == 1) ? Language::EN : Language::AR;
    RefreshUI();
}

void SettingsFrame::OnCountryChanged(wxCommandEvent&) {
    PopulateRegions();
    PopulateCities();
}

void SettingsFrame::OnRegionChanged(wxCommandEvent&) {
    PopulateCities();
}

void SettingsFrame::LoadCurrentSettings() {
    AppConfig config;
    ConfigLoad(config);

    m_currentLang = static_cast<Language>(config.language);
    m_langChoice->SetSelection(config.language);

    // Populate countries
    PopulateCountries();

    // Find and select the saved country
    const auto* country = CitiesDatabase::Instance().FindCountryByIso2(config.countryIso2);
    if (country) {
        for (int i = 0; i < static_cast<int>(m_countryIds.size()); ++i) {
            if (m_countryIds[i] == country->id) {
                m_countryChoice->SetSelection(i);
                break;
            }
        }
    }

    // Populate regions for the selected country
    PopulateRegions();

    if (IsSaudiSelected()) {
        // Find the region that contains the saved Saudi city
        const CityInfo* city = CitiesDatabase::Instance().FindSaudiCityById(config.saudiCityId);
        if (city) {
            // Select the region
            for (int i = 0; i < static_cast<int>(m_saudiRegionIds.size()); ++i) {
                if (m_saudiRegionIds[i] == city->regionId) {
                    m_regionChoice->SetSelection(i);
                    break;
                }
            }

            // Populate cities for that region
            PopulateCities();

            // Select the city
            for (int i = 0; i < static_cast<int>(m_saudiCityIds.size()); ++i) {
                if (m_saudiCityIds[i] == config.saudiCityId) {
                    m_cityChoice->SetSelection(i);
                    break;
                }
            }
        } else {
            PopulateCities();
        }
    } else {
        // Non-Saudi: find and select the saved state
        if (config.stateId > 0) {
            for (int i = 0; i < static_cast<int>(m_stateIds.size()); ++i) {
                if (m_stateIds[i] == config.stateId) {
                    m_regionChoice->SetSelection(i);
                    break;
                }
            }
        }
        PopulateCities();
    }

    // Calculation method: -1=Auto (index 0), 0-7=specific (index 1-8)
    PopulateCalcMethods();
    if (config.calcMethod >= 0 && config.calcMethod < 8)
        m_calcMethodChoice->SetSelection(config.calcMethod + 1);
    else
        m_calcMethodChoice->SetSelection(0); // Auto

    // Hotkey
    m_chkCtrl->SetValue((config.hotkeyModifiers & MOD_CONTROL) != 0);
    m_chkAlt->SetValue((config.hotkeyModifiers & MOD_ALT) != 0);
    m_chkShift->SetValue((config.hotkeyModifiers & MOD_SHIFT) != 0);

    if (config.hotkeyVK >= 'A' && config.hotkeyVK <= 'Z') {
        wchar_t ch = static_cast<wchar_t>(config.hotkeyVK);
        m_keyText->SetValue(std::wstring(1, ch));
    }

    RefreshUI();
}

void SettingsFrame::OnSave(wxCommandEvent&) {
    AppConfig config;

    // Language
    config.language = m_langChoice->GetSelection();

    // Country
    int countrySel = m_countryChoice->GetSelection();
    if (countrySel >= 0 && countrySel < static_cast<int>(m_countryIds.size())) {
        int countryId = m_countryIds[countrySel];
        const auto* country = CitiesDatabase::Instance().FindCountryById(countryId);
        if (country) {
            config.countryIso2 = country->iso2;
        }
    }

    if (IsSaudiSelected()) {
        config.stateId = 0;
        int citySel = m_cityChoice->GetSelection();
        if (citySel >= 0 && citySel < static_cast<int>(m_saudiCityIds.size())) {
            config.saudiCityId = m_saudiCityIds[citySel];
        }
    } else {
        config.saudiCityId = 0;
        int regionSel = m_regionChoice->GetSelection();
        if (regionSel >= 0 && regionSel < static_cast<int>(m_stateIds.size())) {
            config.stateId = m_stateIds[regionSel];
        }
    }

    // Calculation method: selection 0=Auto(-1), 1-8=method 0-7
    int calcSel = m_calcMethodChoice->GetSelection();
    config.calcMethod = (calcSel > 0) ? (calcSel - 1) : -1;

    // Hotkey
    int mod = 0;
    if (m_chkCtrl->GetValue()) mod |= MOD_CONTROL;
    if (m_chkAlt->GetValue()) mod |= MOD_ALT;
    if (m_chkShift->GetValue()) mod |= MOD_SHIFT;
    config.hotkeyModifiers = mod;

    wxString keyStr = m_keyText->GetValue().Upper();
    if (!keyStr.IsEmpty()) {
        config.hotkeyVK = static_cast<int>(keyStr[0]);
    }

    ConfigSave(config);
    Log::Info(L"Settings saved: country=" + std::wstring(config.countryIso2.begin(), config.countryIso2.end())
              + L" stateId=" + std::to_wstring(config.stateId)
              + L" saudiCityId=" + std::to_wstring(config.saudiCityId)
              + L" calcMethod=" + std::to_wstring(config.calcMethod));

    // Re-register hotkey
    HWND hwnd = static_cast<HWND>(wxTheApp->GetTopWindow()->GetHandle());
    m_tray->UnregisterGlobalHotkey(hwnd);
    m_tray->RegisterGlobalHotkey(hwnd, config.hotkeyModifiers, config.hotkeyVK);

    // Update tray tooltip
    m_tray->UpdateTooltip();

    // Use Win32 API directly to hide - CallAfter doesn't always work
    ShowWindow(static_cast<HWND>(GetHandle()), SW_HIDE);
}

WXLRESULT SettingsFrame::MSWWindowProc(WXUINT msg, WXWPARAM wParam, WXLPARAM lParam) {
    if (msg == WM_QUERYENDSESSION)
        return TRUE;  // Allow shutdown
    if (msg == WM_ENDSESSION) {
        if (wParam) wxTheApp->ExitMainLoop();
        return 0;
    }
    return wxFrame::MSWWindowProc(msg, wParam, lParam);
}

void SettingsFrame::OnClose(wxCloseEvent& evt) {
    if (evt.CanVeto()) {
        evt.Veto();
        Hide();
    } else {
        Destroy();
    }
}

bool SettingsFrame::IsSaudiSelected() const {
    int countryId = GetSelectedCountryId();
    return countryId == CitiesDatabase::SAUDI_COUNTRY_ID;
}

int SettingsFrame::GetSelectedCountryId() const {
    int sel = m_countryChoice->GetSelection();
    if (sel >= 0 && sel < static_cast<int>(m_countryIds.size())) {
        return m_countryIds[sel];
    }
    return -1;
}
