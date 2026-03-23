#include <wx/wx.h>
#include <windows.h>
#include "TrayIcon.h"
#include "SettingsFrame.h"
#include "Speech.h"
#include "Config.h"
#include "Cities.h"
#include "DataManager.h"
#include "Updater.h"
#include "Lang.h"
#include "Log.h"

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
        if (msg == WM_QUERYENDSESSION)
            return TRUE;  // Allow shutdown
        if (msg == WM_ENDSESSION) {
            if (wParam) wxTheApp->ExitMainLoop();
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
        // Prevent multiple instances using a named mutex
        m_singleInstanceMutex = CreateMutexW(nullptr, TRUE, L"Global\\NidaaPrayerTimesApp");
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            // Another instance is already running - bring its settings window to front
            HWND existing = FindWindowW(nullptr, L"\x0625\x0639\x062F\x0627\x062F\x0627\x062A \x0646\x062F\x0627\x0621");
            if (!existing)
                existing = FindWindowW(nullptr, L"Nidaa Settings");
            if (existing) {
                SetForegroundWindow(existing);
                if (IsIconic(existing)) ShowWindow(existing, SW_RESTORE);
            }
            if (m_singleInstanceMutex) CloseHandle(m_singleInstanceMutex);
            m_singleInstanceMutex = nullptr;
            return false;
        }

        Log::Init();
        SpeechInit();

        auto& dm = DataManager::Instance();
        bool firstLaunch = IsFirstLaunch();

        // Load config early so we know the language
        AppConfig config;
        ConfigLoad(config);
        Language lang = static_cast<Language>(config.language);

        // Download/cache data if needed
        if (!dm.IsWorldDataCached()) {
            Log::Info(L"World data not cached, downloading...");
            SpeechSay(Lang::DownloadingData(lang));
            if (!dm.DownloadWorldData()) {
                Log::Error(L"Failed to download world data");
                SpeechSay(Lang::DownloadFailed(lang));
            }
        }

        // Load data
        auto& db = CitiesDatabase::Instance();
        if (!db.LoadWorldData(dm.GetCacheDir()))
            Log::Error(L"Failed to load world data from: " + dm.GetCacheDir());
        else
            Log::Info(L"World data loaded: " + std::to_wstring(db.GetCountries().size()) + L" countries");

        if (!db.LoadSaudiData(dm.GetCacheDir()))
            Log::Error(L"Failed to load Saudi data from: " + dm.GetCacheDir());
        else
            Log::Info(L"Saudi data loaded: " + std::to_wstring(db.GetSaudiRegions().size()) + L" regions");

        m_trayIcon = new NidaaTrayIcon();

        m_hotkeyFrame = new HotkeyFrame(m_trayIcon);
        m_hotkeyFrame->Show(false);

        m_settingsFrame = new SettingsFrame(m_trayIcon);
        m_trayIcon->SetSettingsFrame(m_settingsFrame);

        HWND hwnd = static_cast<HWND>(m_hotkeyFrame->GetHandle());
        m_trayIcon->RegisterGlobalHotkey(hwnd, config.hotkeyModifiers, config.hotkeyVK);

        SetTopWindow(m_hotkeyFrame);

        // Check for updates - download installer and run it
        {
            std::wstring installerPath;
            if (Updater::CheckForUpdate(installerPath)) {
                SpeechSay(Lang::UpdateAvailable(lang));
                // Launch installer silently and exit
                ShellExecuteW(nullptr, L"open", installerPath.c_str(),
                              L"/SILENT", nullptr, SW_SHOWNORMAL);
                return false;  // Exit the app so installer can replace files
            }
        }

        // Only show settings on first launch or with --settings flag
        if (firstLaunch || (argc > 1 && wxString(argv[1]) == "--settings")) {
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
        if (m_singleInstanceMutex) {
            ReleaseMutex(m_singleInstanceMutex);
            CloseHandle(m_singleInstanceMutex);
            m_singleInstanceMutex = nullptr;
        }
        return wxApp::OnExit();
    }

private:
    NidaaTrayIcon* m_trayIcon = nullptr;
    HotkeyFrame* m_hotkeyFrame = nullptr;
    SettingsFrame* m_settingsFrame = nullptr;
    HANDLE m_singleInstanceMutex = nullptr;
};

wxIMPLEMENT_APP(NidaaApp);
