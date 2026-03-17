#pragma once
#include <string>
#include <ctime>
#include "Lang.h"
#include "CalcMethod.h"

struct PrayerSchedule {
    double fajr;
    double sunrise;
    double dhuhr;
    double asr;
    double maghrib;
    double isha;
};

// Names array index
enum Prayer { FAJR = 0, SUNRISE, DHUHR, ASR, MAGHRIB, ISHA, PRAYER_COUNT };

class PrayerTimesCalculator {
public:
    PrayerSchedule Calculate(int year, int month, int day,
                             double latitude, double longitude, double timezone,
                             CalcMethodId method = CalcMethodId::UMM_AL_QURA);

    static double GetPrayerTime(const PrayerSchedule& schedule, int index);
    static std::wstring FormatTime(double hours);
    static std::wstring GetPrayerName(int index, Language lang = Language::AR);

    struct NextPrayerInfo {
        int nextPrayer;
        double nextTime;
        int prevPrayer;
        double prevTime;
        int remainingMinutes;
        int elapsedMinutes;
    };
    static NextPrayerInfo FindNextPrayer(const PrayerSchedule& schedule, double currentHours);

private:
    double JulianDate(int year, int month, int day);
    double SunDeclination(double jd);
    double EquationOfTime(double jd);
    double AsrFactor(double latitude, double declination);
};
