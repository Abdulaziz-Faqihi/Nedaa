#pragma once
#include <string>

bool SpeechInit();
void SpeechShutdown();
bool SpeechSay(const std::wstring& text);
