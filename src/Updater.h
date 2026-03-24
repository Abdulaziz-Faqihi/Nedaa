#pragma once
#include <string>

// Current application version
#define NIDAA_VERSION "1.12.0"

namespace Updater {
    // Check GitHub releases for a newer version.
    // If an update is found, downloads the installer to a temp path and returns true.
    bool CheckForUpdate(std::wstring& installerPath);
}
