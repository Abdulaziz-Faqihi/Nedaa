#include "Speech.h"
#include <windows.h>

using SpeechSayFunc = int(__cdecl*)(const wchar_t*, int);

static HMODULE s_hModule = nullptr;
static SpeechSayFunc s_speechSay = nullptr;

bool SpeechInit() {
    if (s_hModule) return true;

    s_hModule = LoadLibraryW(L"UniversalSpeech.dll");
    if (!s_hModule) return false;

    s_speechSay = reinterpret_cast<SpeechSayFunc>(
        GetProcAddress(s_hModule, "speechSay"));

    if (!s_speechSay) {
        FreeLibrary(s_hModule);
        s_hModule = nullptr;
        return false;
    }
    return true;
}

void SpeechShutdown() {
    if (s_hModule) {
        FreeLibrary(s_hModule);
        s_hModule = nullptr;
        s_speechSay = nullptr;
    }
}

bool SpeechSay(const std::wstring& text) {
    if (!s_speechSay) return false;
    s_speechSay(text.c_str(), 1); // interrupt = 1
    return true;
}
