#include "SupportDialog.h"
#include "LogSender.h"
#include "Log.h"
#include "Speech.h"
#include <wx/sizer.h>

SupportDialog::SupportDialog(wxWindow* parent, Language lang)
    : wxDialog(parent, wxID_ANY,
               Lang::SupportTitle(lang),
               wxDefaultPosition, wxSize(450, 550),
               wxDEFAULT_DIALOG_STYLE)
    , m_lang(lang)
{
    auto* panel = new wxPanel(this);
    auto* sizer = new wxBoxSizer(wxVERTICAL);

    // First name
    {
        auto* lbl = new wxStaticText(panel, wxID_ANY, Lang::SupportFirstName(lang));
        sizer->Add(lbl, 0, wxALL, 5);
        m_firstName = new wxTextCtrl(panel, wxID_ANY);
        m_firstName->SetName(Lang::SupportFirstName(lang));
        sizer->Add(m_firstName, 0, wxEXPAND | wxALL, 5);
    }

    // Last name
    {
        auto* lbl = new wxStaticText(panel, wxID_ANY, Lang::SupportLastName(lang));
        sizer->Add(lbl, 0, wxALL, 5);
        m_lastName = new wxTextCtrl(panel, wxID_ANY);
        m_lastName->SetName(Lang::SupportLastName(lang));
        sizer->Add(m_lastName, 0, wxEXPAND | wxALL, 5);
    }

    // Type (problem / feature suggestion)
    {
        auto* lbl = new wxStaticText(panel, wxID_ANY, Lang::SupportType(lang));
        sizer->Add(lbl, 0, wxALL, 5);
        m_typeChoice = new wxChoice(panel, wxID_ANY);
        m_typeChoice->SetName(Lang::SupportType(lang));
        m_typeChoice->Append(Lang::SupportTypeProblem(lang));
        m_typeChoice->Append(Lang::SupportTypeFeature(lang));
        m_typeChoice->SetSelection(0);
        sizer->Add(m_typeChoice, 0, wxEXPAND | wxALL, 5);
    }

    // Contact method
    {
        auto* lbl = new wxStaticText(panel, wxID_ANY, Lang::SupportContactMethod(lang));
        sizer->Add(lbl, 0, wxALL, 5);
        m_contactMethodChoice = new wxChoice(panel, ID_CONTACT_METHOD);
        m_contactMethodChoice->SetName(Lang::SupportContactMethod(lang));
        m_contactMethodChoice->Append(Lang::SupportContactWhatsApp(lang));
        m_contactMethodChoice->Append(Lang::SupportContactTelegram(lang));
        m_contactMethodChoice->Append(Lang::SupportContactEmail(lang));
        m_contactMethodChoice->SetSelection(0);
        sizer->Add(m_contactMethodChoice, 0, wxEXPAND | wxALL, 5);
        m_contactMethodChoice->Bind(wxEVT_CHOICE, &SupportDialog::OnContactMethodChanged, this, ID_CONTACT_METHOD);
    }

    // Contact details
    {
        m_lblContactDetails = new wxStaticText(panel, wxID_ANY, Lang::SupportContactDetailsWhatsApp(lang));
        sizer->Add(m_lblContactDetails, 0, wxALL, 5);
        m_contactDetails = new wxTextCtrl(panel, wxID_ANY);
        m_contactDetails->SetName(Lang::SupportContactDetailsWhatsApp(lang));
        sizer->Add(m_contactDetails, 0, wxEXPAND | wxALL, 5);
    }

    // Subject
    {
        auto* lbl = new wxStaticText(panel, wxID_ANY, Lang::SupportSubject(lang));
        sizer->Add(lbl, 0, wxALL, 5);
        m_subject = new wxTextCtrl(panel, wxID_ANY);
        m_subject->SetName(Lang::SupportSubject(lang));
        sizer->Add(m_subject, 0, wxEXPAND | wxALL, 5);
    }

    // Message
    {
        auto* lbl = new wxStaticText(panel, wxID_ANY, Lang::SupportMessage(lang));
        sizer->Add(lbl, 0, wxALL, 5);
        m_message = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
                                   wxDefaultPosition, wxSize(-1, 100),
                                   wxTE_MULTILINE);
        m_message->SetName(Lang::SupportMessage(lang));
        sizer->Add(m_message, 1, wxEXPAND | wxALL, 5);
    }

    // Attach logs checkbox
    {
        m_attachLogs = new wxCheckBox(panel, wxID_ANY, Lang::SupportAttachLogs(lang));
        sizer->Add(m_attachLogs, 0, wxALL, 5);
    }

    // Send button
    {
        m_sendBtn = new wxButton(panel, ID_SEND, Lang::SupportSendButton(lang));
        sizer->Add(m_sendBtn, 0, wxALIGN_CENTER | wxALL, 10);
        m_sendBtn->Bind(wxEVT_BUTTON, &SupportDialog::OnSend, this);
    }

    panel->SetSizer(sizer);

    if (lang == Language::AR) {
        SetLayoutDirection(wxLayout_RightToLeft);
    }
}

void SupportDialog::OnContactMethodChanged(wxCommandEvent&) {
    UpdateContactHint();
}

void SupportDialog::UpdateContactHint() {
    int sel = m_contactMethodChoice->GetSelection();
    const wchar_t* label = nullptr;
    switch (sel) {
        case 0: label = Lang::SupportContactDetailsWhatsApp(m_lang); break;
        case 1: label = Lang::SupportContactDetailsTelegram(m_lang); break;
        case 2: label = Lang::SupportContactDetailsEmail(m_lang); break;
        default: label = Lang::SupportContactDetailsWhatsApp(m_lang); break;
    }
    m_lblContactDetails->SetLabel(label);
    m_contactDetails->SetName(label);
}

void SupportDialog::OnSend(wxCommandEvent&) {
    wxString firstName = m_firstName->GetValue().Trim();
    wxString lastName = m_lastName->GetValue().Trim();
    wxString contact = m_contactDetails->GetValue().Trim();
    wxString subject = m_subject->GetValue().Trim();
    wxString message = m_message->GetValue().Trim();

    if (firstName.IsEmpty() || subject.IsEmpty() || message.IsEmpty()) {
        SpeechSay(Lang::SupportFillRequired(m_lang));
        return;
    }

    // Build the support message
    int typeSel = m_typeChoice->GetSelection();
    wxString typeStr = (typeSel == 0)
        ? Lang::SupportTypeProblem(m_lang)
        : Lang::SupportTypeFeature(m_lang);

    int contactSel = m_contactMethodChoice->GetSelection();
    wxString contactMethod;
    switch (contactSel) {
        case 0: contactMethod = L"WhatsApp"; break;
        case 1: contactMethod = L"Telegram"; break;
        case 2: contactMethod = L"Email"; break;
    }

    std::wstring fullMessage;
    fullMessage += L"--- Support Request ---\n";
    fullMessage += L"Name: " + firstName.ToStdWstring() + L" " + lastName.ToStdWstring() + L"\n";
    fullMessage += L"Type: " + typeStr.ToStdWstring() + L"\n";
    fullMessage += L"Contact: " + contactMethod.ToStdWstring() + L" - " + contact.ToStdWstring() + L"\n";
    fullMessage += L"Subject: " + subject.ToStdWstring() + L"\n";
    fullMessage += L"Message:\n" + message.ToStdWstring() + L"\n";

    SpeechSay(Lang::SendingLog(m_lang));
    m_sendBtn->Enable(false);

    bool ok;
    if (m_attachLogs->GetValue()) {
        ok = LogSender::SendLogToTelegram(fullMessage);
    } else {
        ok = LogSender::SendTextToTelegram(fullMessage);
    }

    m_sendBtn->Enable(true);

    if (ok) {
        SpeechSay(Lang::SupportSentSuccess(m_lang));
        EndModal(wxID_OK);
    } else {
        SpeechSay(Lang::LogSentFailed(m_lang));
    }
}
