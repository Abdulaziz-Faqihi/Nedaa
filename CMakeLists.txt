cmake_minimum_required(VERSION 3.20)
project(Nidaa LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find wxWidgets via vcpkg
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# nlohmann-json for config persistence
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(Nidaa WIN32
    src/main.cpp
    src/TrayIcon.cpp
    src/SettingsFrame.cpp
    src/PrayerTimes.cpp
    src/HijriDate.cpp
    src/Speech.cpp
    src/Config.cpp
    src/Cities.cpp
    src/Lang.cpp
    src/CalcMethod.cpp
    src/DataManager.cpp
    src/Updater.cpp
)

target_include_directories(Nidaa PRIVATE src)
target_link_libraries(Nidaa PRIVATE ${wxWidgets_LIBRARIES} nlohmann_json::nlohmann_json urlmon wininet)

# Copy UniversalSpeech DLLs to output directory
set(US_DLL_DIR "D:/my Files/My projects/Programming/C++/Libraries/UniversalSpeech/bin/64")
add_custom_command(TARGET Nidaa POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${US_DLL_DIR}/UniversalSpeech.dll"
        "${US_DLL_DIR}/nvdaControllerClient.dll"
        "${US_DLL_DIR}/ZDSRAPI.dll"
        $<TARGET_FILE_DIR:Nidaa>
)

# Copy JSON data files to output directory
add_custom_command(TARGET Nidaa POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/sa_cities.json"
        "${CMAKE_SOURCE_DIR}/sa_regions_lite.json"
        "${CMAKE_SOURCE_DIR}/countries.json"
        "${CMAKE_SOURCE_DIR}/states.json"
        $<TARGET_FILE_DIR:Nidaa>
)
