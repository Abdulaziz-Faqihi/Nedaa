#pragma once
#include <wx/wx.h>
#include <wx/choice.h>
#include "Lang.h"

class SupportDialog : public wxDialog {
public:
    SupportDialog(wxWindow* parent, Language lang);

private:
    void OnSend(wxCommandEvent& evt);
    void OnContactMethodChanged(wxCommandEvent& evt);
    void UpdateContactHint();

    wxTextCtrl* m_firstName = nullptr;
    wxTextCtrl* m_lastName = nullptr;
    wxChoice* m_typeChoice = nullptr;
    wxChoice* m_contactMethodChoice = nullptr;
    wxTextCtrl* m_contactDetails = nullptr;
    wxTextCtrl* m_subject = nullptr;
    wxTextCtrl* m_message = nullptr;
    wxCheckBox* m_attachLogs = nullptr;
    wxButton* m_sendBtn = nullptr;
    wxStaticText* m_lblContactDetails = nullptr;
    bool m_sending = false;

    Language m_lang;

    enum { ID_SEND = 30001, ID_CONTACT_METHOD = 30002 };
};
