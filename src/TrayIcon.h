#pragma once
#include <wx/taskbar.h>
#include <wx/frame.h>

class SettingsFrame;

class NidaaTrayIcon : public wxTaskBarIcon {
public:
    NidaaTrayIcon();
    ~NidaaTrayIcon();

    void OnHotkeyPressed();
    void RegisterGlobalHotkey(void* hwnd, int modifiers, int vk);
    void UnregisterGlobalHotkey(void* hwnd);

    void SetSettingsFrame(SettingsFrame* frame) { m_settingsFrame = frame; }
    void UpdateTooltip();

private:
    wxMenu* CreatePopupMenu() override;

    void OnSettings(wxCommandEvent& evt);
    void OnCheckUpdate(wxCommandEvent& evt);
    void OnExit(wxCommandEvent& evt);

    SettingsFrame* m_settingsFrame = nullptr;

    enum { ID_SETTINGS = 10001, ID_CHECK_UPDATE, ID_EXIT };
};
