\# Role

Act as an expert C++ Windows Desktop developer. I want to build a lightweight, native offline Windows background application named "Nidaa" using C++, wxWidgets, and CMake.



\# Project Requirements \& Constraints



\## 1. Environment \& Build System

\- \*\*Project Name:\*\* Nidaa (The final executable should be `Nidaa.exe`).

\- \*\*Compiler:\*\* MSVC (Visual Studio Build Tools only, no full IDE).

\- \*\*Build System:\*\* CMake.

\- \*\*Package Manager:\*\* vcpkg located exactly at `D:\\my Files\\My projects\\Programming\\C++\\Libraries\\vcpkg`.

\- \*\*Requirement:\*\* You must provide a complete `CMakeLists.txt` that correctly utilizes the vcpkg toolchain to find and link `wxWidgets` and the `adhan` library.



\## 2. Core Functionality

\- \*\*System Tray:\*\* The app should run entirely in the background as a system tray icon (`wxTaskBarIcon`). It should \*\*not\*\* show a main window on startup.

\- \*\*Context Menu:\*\* Right-clicking the tray icon should show a menu with "Settings" and "Exit".

\- \*\*Global Hotkey:\*\* Register a global hotkey (e.g., `Ctrl + Alt + P`). When pressed from anywhere in Windows, the app should trigger the calculation and speech function.

\- \*\*Prayer Times (Offline):\*\* Use the `adhan` C++ library (from Batool Apps, installable via vcpkg) using the Umm al-Qura calculation method.

\- \*\*Hijri Date:\*\* Use the native Windows API (Win32) to fetch the current Hijri date using the `CAL\_UMALQURA` identifier. Do not use heavy external libraries like ICU for this.

\- \*\*Time Logic:\*\* When the hotkey is pressed, calculate the next prayer time. Determine the remaining time until the next prayer. If the current prayer time has recently passed, also calculate the elapsed time since that prayer.



\## 3. Screen Reader Integration (UniversalSpeech)

\- I am using the UniversalSpeech API to send the final text directly to my screen reader.

\- The 64-bit DLLs are located here:

&nbsp; - `D:\\my Files\\My projects\\Programming\\C++\\Libraries\\UniversalSpeech\\bin\\64\\UniversalSpeech.dll`

&nbsp; - `D:\\my Files\\My projects\\Programming\\C++\\Libraries\\UniversalSpeech\\bin\\64\\nvdaControllerClient.dll`

&nbsp; - `D:\\my Files\\My projects\\Programming\\C++\\Libraries\\UniversalSpeech\\bin\\64\\ZDSRAPI.dll`

\- \*\*Requirement:\*\* Since I only have the DLLs, please use `LoadLibraryW` and `GetProcAddress` at runtime to load the `speechSay` (or equivalent export) function from `UniversalSpeech.dll`. Initialize the speech engine when the app starts.

\- \*\*Output Format:\*\* When the hotkey is pressed, format a clean, clear text string containing: The Hijri Date, the next prayer name, time remaining for it, and (if applicable) time elapsed since the last prayer. Pass this string to the dynamically loaded UniversalSpeech function.



\## 4. Settings UI \& Saudi Cities Database

\- A simple `wxFrame` that opens only when clicking "Settings" from the tray menu. Ensure all UI controls are fully accessible for screen reader users.

\- \*\*City Selection:\*\* Provide an accessible combo box (`wxComboBox` or `wxChoice`) populated with major regions and cities in Saudi Arabia. The app should map the selected city to its hardcoded Latitude and Longitude offline. Default to Makkah on the first launch.

\- \*\*Hotkey Settings:\*\* Allow the user to change the Global Hotkey combination.

\- \*\*Persistence:\*\* Save the selected city and hotkey settings locally (e.g., in a simple `config.ini` or `.json` file) so they persist across restarts.



\# Expected Output

Please provide:

1\. The exact CMake command line instructions to generate and build the "Nidaa" project using my specific vcpkg toolchain path.

2\. The complete `CMakeLists.txt`.

3\. The C++ source code files (`main.cpp`, `TrayIcon.cpp`, `SettingsFrame.cpp`, etc.) structured cleanly. Keep performance high and memory footprint minimal.

