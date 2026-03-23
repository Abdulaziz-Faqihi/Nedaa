#pragma once
#include <wx/wx.h>
#include <wx/choice.h>
#include "Lang.h"

class NidaaTrayIcon;

class SettingsFrame : public wxFrame {
public:
    SettingsFrame(NidaaTrayIcon* tray);
    void RefreshUI();

protected:
    WXLRESULT MSWWindowProc(WXUINT msg, WXWPARAM wParam, WXLPARAM lParam) override;

private:
    void OnSave(wxCommandEvent& evt);
    void OnClose(wxCloseEvent& evt);
    void OnLanguageChanged(wxCommandEvent& evt);
    void OnCountryChanged(wxCommandEvent& evt);
    void OnRegionChanged(wxCommandEvent& evt);
    void PopulateCountries();
    void PopulateRegions();
    void PopulateCities();
    void PopulateCalcMethods();
    void OnCheckUpdate(wxCommandEvent& evt);
    void OnSendLog(wxCommandEvent& evt);
    void LoadCurrentSettings();

    bool IsSaudiSelected() const;
    int GetSelectedCountryId() const;

    wxChoice* m_langChoice = nullptr;
    wxChoice* m_countryChoice = nullptr;
    wxChoice* m_regionChoice = nullptr;
    wxChoice* m_cityChoice = nullptr;
    wxCheckBox* m_chkCtrl = nullptr;
    wxCheckBox* m_chkAlt = nullptr;
    wxCheckBox* m_chkShift = nullptr;
    wxTextCtrl* m_keyText = nullptr;
    NidaaTrayIcon* m_tray = nullptr;

    wxStaticText* m_lblCountry = nullptr;
    wxStaticText* m_lblRegion = nullptr;
    wxStaticText* m_lblCity = nullptr;
    wxStaticText* m_lblCalcMethod = nullptr;
    wxStaticText* m_lblHotkey = nullptr;
    wxStaticText* m_lblKey = nullptr;
    wxStaticText* m_lblLang = nullptr;
    wxChoice* m_calcMethodChoice = nullptr;
    wxButton* m_saveBtn = nullptr;
    wxButton* m_updateBtn = nullptr;
    wxButton* m_sendLogBtn = nullptr;

    Language m_currentLang = Language::AR;

    // Track IDs for combos
    std::vector<int> m_countryIds;     // parallel to country combo items
    std::vector<int> m_stateIds;       // parallel to region combo items (non-Saudi)
    std::vector<int> m_saudiRegionIds; // parallel to region combo items (Saudi)
    std::vector<int> m_saudiCityIds;   // parallel to city combo items (Saudi)

    enum { ID_SAVE = 20001, ID_LANG = 20002, ID_COUNTRY = 20003, ID_REGION = 20004, ID_CHECK_UPDATE = 20005, ID_SEND_LOG = 20006 };
};
