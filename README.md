# Nidaa (نداء) - Prayer Times for Windows

Nidaa is a lightweight Windows tray application that announces Islamic prayer times via screen reader. It runs silently in the system tray and speaks the current/next prayer time, remaining time, and Hijri date when you press a global hotkey.

## Features

- **Screen reader accessible** - Announces prayer times through any screen reader using UniversalSpeech (NVDA, JAWS, SAPI, etc.)
- **Global hotkey** - Press Ctrl+Alt+P (configurable) from anywhere to hear prayer times
- **250 countries supported** - Automatic calculation method selection per country
- **Detailed Saudi Arabia data** - 13 regions and 4581 cities with precise coordinates
- **8 calculation methods** - Umm al-Qura, Muslim World League, Egyptian, Karachi, ISNA, Diyanet, Tehran, Singapore
- **Hijri date** - Always displays the Hijri (Islamic) date in both Arabic and English
- **Bilingual** - Full Arabic and English interface
- **Offline** - All prayer time calculations are done locally, no internet required after first launch
- **Lightweight** - Minimal resource usage, runs quietly in the system tray

## How It Works

1. On first launch, Nidaa downloads location data and opens the settings window
2. Select your country, region, and city (Saudi Arabia has detailed city-level data)
3. Configure your preferred language and hotkey
4. Nidaa sits in the system tray - press the hotkey anytime to hear:
   - Today's Hijri date
   - All six prayer times (Fajr, Sunrise, Dhuhr, Asr, Maghrib, Isha)
   - The next upcoming prayer and time remaining
   - Or time elapsed since the last prayer (after Isha)

## Calculation Methods

Each country is automatically mapped to its standard calculation method:

| Method | Used By |
|--------|---------|
| Umm al-Qura | Saudi Arabia, Gulf states |
| Muslim World League | Europe, Americas, most of Asia |
| Egyptian General Authority | Egypt, Africa, parts of Middle East |
| University of Islamic Sciences, Karachi | Pakistan, Bangladesh, Afghanistan |
| Islamic Society of North America (ISNA) | USA, Canada |
| Diyanet | Turkey, Central Asia |
| Institute of Geophysics, Tehran | Iran |
| Singapore/MUIS | Singapore, Malaysia, Indonesia |

## Building from Source

### Prerequisites

- Visual Studio Build Tools 2022 (or later) with C++ workload
- [CMake](https://cmake.org/) 3.20+
- [vcpkg](https://github.com/microsoft/vcpkg) with the following packages:
  - `wxwidgets:x64-windows`
  - `nlohmann-json:x64-windows`
- [UniversalSpeech](https://github.com/qtnc/UniversalSpeech) DLLs (64-bit)

### Build Steps

```bash
# Configure
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build build --config Release
```

### Building the Installer

Requires [Inno Setup](https://jrsoftware.org/isinfo.php) installed:

```bash
iscc installer.iss
```

The installer will be created in the `installer_output/` directory.

## Data Sources

- **World countries and states**: [dr5hn/countries-states-cities-database](https://github.com/dr5hn/countries-states-cities-database) (250 countries, 5296 states/regions)
- **Saudi Arabia cities**: [homaily/Saudi-Arabia-Regions-Cities-and-Districts](https://github.com/homaily/Saudi-Arabia-Regions-Cities-and-Districts) (13 regions, 4581 cities)

## Tech Stack

- **C++17** with MSVC
- **wxWidgets 3.2** - UI framework and system tray
- **nlohmann/json** - Configuration and data parsing
- **UniversalSpeech** - Screen reader output
- **Win32 API** - Global hotkeys, Hijri date conversion
- **Inno Setup** - Windows installer

## License

This project is open source.
