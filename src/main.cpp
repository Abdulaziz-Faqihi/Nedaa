#include <wx/wx.h>
#include <windows.h>
#include "TrayIcon.h"
#include "SettingsFrame.h"
#include "Speech.h"
#include "Config.h"
#include "Cities.h"
#include "DataManager.h"

static const int HOTKEY_ID = 1;

// Hidden window to receive WM_HOTKEY messages
class HotkeyFrame : public wxFrame {
public:
    HotkeyFrame(NidaaTrayIcon* tray)
        : wxFrame(nullptr, wxID_ANY, L"NidaaHotkey", wxDefaultPosition, wxSize(0, 0),
                  wxFRAME_NO_TASKBAR)
        , m_tray(tray)
    {
        Show(false);
    }

protected:
    WXLRESULT MSWWindowProc(WXUINT msg, WXWPARAM wParam, WXLPARAM lParam) override {
        if (msg == WM_HOTKEY && static_cast<int>(wParam) == HOTKEY_ID) {
            if (m_tray) m_tray->OnHotkeyPressed();
            return 0;
        }
        return wxFrame::MSWWindowProc(msg, wParam, lParam);
    }

private:
    NidaaTrayIcon* m_tray;
};

class NidaaApp : public wxApp {
public:
    bool OnInit() override {
        SpeechInit();

        auto& dm = DataManager::Instance();
        bool firstLaunch = IsFirstLaunch();

        // Download/cache data if needed
        if (!dm.IsWorldDataCached()) {
            SpeechSay(Lang::DownloadingData(Language::AR));
            if (!dm.DownloadWorldData()) {
                SpeechSay(Lang::DownloadFailed(Language::AR));
                // Continue anyway - Saudi bundled data may still work
            }
        }

        // Load data
        auto& db = CitiesDatabase::Instance();
        db.LoadWorldData(dm.GetCacheDir());
        db.LoadSaudiData(dm.GetCacheDir());

        AppConfig config;
        ConfigLoad(config);

        m_trayIcon = new NidaaTrayIcon();

        m_hotkeyFrame = new HotkeyFrame(m_trayIcon);
        m_hotkeyFrame->Show(false);

        m_settingsFrame = new SettingsFrame(m_trayIcon);
        m_trayIcon->SetSettingsFrame(m_settingsFrame);

        HWND hwnd = static_cast<HWND>(m_hotkeyFrame->GetHandle());
        m_trayIcon->RegisterGlobalHotkey(hwnd, config.hotkeyModifiers, config.hotkeyVK);

        SetTopWindow(m_hotkeyFrame);

        // Show settings on first launch
        if (firstLaunch) {
            m_settingsFrame->Show(true);
            m_settingsFrame->Raise();
        }

        return true;
    }

    int OnExit() override {
        if (m_hotkeyFrame) {
            HWND hwnd = static_cast<HWND>(m_hotkeyFrame->GetHandle());
            m_trayIcon->UnregisterGlobalHotkey(hwnd);
        }
        delete m_trayIcon;
        m_trayIcon = nullptr;
        SpeechShutdown();
        return wxApp::OnExit();
    }

private:
    NidaaTrayIcon* m_trayIcon = nullptr;
    HotkeyFrame* m_hotkeyFrame = nullptr;
    SettingsFrame* m_settingsFrame = nullptr;
};

wxIMPLEMENT_APP(NidaaApp);
