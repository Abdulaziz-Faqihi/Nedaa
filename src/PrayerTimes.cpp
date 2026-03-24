#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "PrayerTimes.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

static double Radians(double deg) { return deg * M_PI / 180.0; }
static double Degrees(double rad) { return rad * 180.0 / M_PI; }

static double FixHour(double h) {
    h = fmod(h, 24.0);
    return h < 0 ? h + 24.0 : h;
}

double PrayerTimesCalculator::JulianDate(int year, int month, int day) {
    if (month <= 2) { year--; month += 12; }
    double A = floor(year / 100.0);
    double B = 2 - A + floor(A / 4.0);
    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + B - 1524.5;
}

double PrayerTimesCalculator::SunDeclination(double jd) {
    double D = jd - 2451545.0;
    double g = Radians(fmod(357.529 + 0.98560028 * D, 360.0));
    double q = fmod(280.459 + 0.98564736 * D, 360.0);
    double L = Radians(fmod(q + 1.915 * sin(g) + 0.020 * sin(2 * g), 360.0));
    double e = Radians(23.439 - 0.00000036 * D);
    return Degrees(asin(sin(e) * sin(L)));
}

double PrayerTimesCalculator::EquationOfTime(double jd) {
    double D = jd - 2451545.0;
    double g = Radians(fmod(357.529 + 0.98560028 * D, 360.0));
    double q = fmod(280.459 + 0.98564736 * D, 360.0);
    double L = Radians(fmod(q + 1.915 * sin(g) + 0.020 * sin(2 * g), 360.0));
    double e = Radians(23.439 - 0.00000036 * D);
    double RA = Degrees(atan2(cos(e) * sin(L), cos(L))) / 15.0;
    RA = FixHour(RA);
    return q / 15.0 - RA;
}

double PrayerTimesCalculator::AsrFactor(double latitude, double declination) {
    double angle = Degrees(atan(1.0 / (1.0 + tan(Radians(fabs(latitude - declination))))));
    return angle;
}

PrayerSchedule PrayerTimesCalculator::Calculate(int year, int month, int day,
                                                 double latitude, double longitude, double timezone,
                                                 CalcMethodId method) {
    const CalcMethodParams& params = GetCalcMethod(method);

    double jd = JulianDate(year, month, day);
    double decl = SunDeclination(jd);
    double eqt = EquationOfTime(jd);

    double dhuhr = FixHour(12.0 + timezone - longitude / 15.0 - eqt);

    // Sunrise
    double sunriseHA = Degrees(acos(
        (-sin(Radians(0.833)) - sin(Radians(latitude)) * sin(Radians(decl))) /
        (cos(Radians(latitude)) * cos(Radians(decl)))
    )) / 15.0;

    PrayerSchedule s;
    s.sunrise = FixHour(dhuhr - sunriseHA);
    s.dhuhr = FixHour(dhuhr);

    // Fajr
    double fajrHA = Degrees(acos(
        (-sin(Radians(params.fajrAngle)) - sin(Radians(latitude)) * sin(Radians(decl))) /
        (cos(Radians(latitude)) * cos(Radians(decl)))
    )) / 15.0;
    s.fajr = FixHour(dhuhr - fajrHA);

    // Asr (standard - Shafi'i: shadow = length + 1x object)
    double asrCotAngle = 1.0 + tan(Radians(fabs(latitude - decl)));
    double asrElevation = Degrees(atan(1.0 / asrCotAngle));
    double asrHA = Degrees(acos(
        (sin(Radians(asrElevation)) - sin(Radians(latitude)) * sin(Radians(decl))) /
        (cos(Radians(latitude)) * cos(Radians(decl)))
    )) / 15.0;
    s.asr = FixHour(dhuhr + asrHA);

    // Maghrib (sunset)
    double sunsetHA = Degrees(acos(
        (-sin(Radians(0.833)) - sin(Radians(latitude)) * sin(Radians(decl))) /
        (cos(Radians(latitude)) * cos(Radians(decl)))
    )) / 15.0;
    s.maghrib = FixHour(dhuhr + sunsetHA);

    // Isha
    if (params.ishaMinutes > 0) {
        // Fixed minutes after Maghrib (e.g., Umm al-Qura: 90 min)
        s.isha = FixHour(s.maghrib + params.ishaMinutes / 60.0);
    } else {
        // Angle-based
        double ishaHA = Degrees(acos(
            (-sin(Radians(params.ishaAngle)) - sin(Radians(latitude)) * sin(Radians(decl))) /
            (cos(Radians(latitude)) * cos(Radians(decl)))
        )) / 15.0;
        s.isha = FixHour(dhuhr + ishaHA);
    }

    return s;
}

double PrayerTimesCalculator::GetPrayerTime(const PrayerSchedule& schedule, int index) {
    switch (index) {
        case FAJR:    return schedule.fajr;
        case SUNRISE: return schedule.sunrise;
        case DHUHR:   return schedule.dhuhr;
        case ASR:     return schedule.asr;
        case MAGHRIB: return schedule.maghrib;
        case ISHA:    return schedule.isha;
        default:      return 0;
    }
}

std::wstring PrayerTimesCalculator::FormatTime(double hours) {
    hours = FixHour(hours);
    int h = static_cast<int>(hours);
    int m = static_cast<int>((hours - h) * 60.0 + 0.5);
    if (m >= 60) { h++; m -= 60; }

    // 12-hour format
    const wchar_t* period = (h >= 12) ? L" PM" : L" AM";
    int h12 = h % 12;
    if (h12 == 0) h12 = 12;

    std::wostringstream ss;
    ss << h12 << L":" << std::setfill(L'0') << std::setw(2) << m << period;
    return ss.str();
}

std::wstring PrayerTimesCalculator::GetPrayerName(int index, Language lang) {
    return Lang::PrayerName(index, lang);
}

PrayerTimesCalculator::NextPrayerInfo PrayerTimesCalculator::FindNextPrayer(
    const PrayerSchedule& schedule, double currentHours) {

    NextPrayerInfo info{};
    int order[] = { FAJR, SUNRISE, DHUHR, ASR, MAGHRIB, ISHA };

    info.nextPrayer = -1;
    info.prevPrayer = -1;

    for (int i = 0; i < PRAYER_COUNT; ++i) {
        double t = GetPrayerTime(schedule, order[i]);
        if (currentHours < t) {
            info.nextPrayer = order[i];
            info.nextTime = t;
            info.remainingMinutes = static_cast<int>((t - currentHours) * 60.0);
            if (i > 0) {
                info.prevPrayer = order[i - 1];
                info.prevTime = GetPrayerTime(schedule, order[i - 1]);
                info.elapsedMinutes = static_cast<int>((currentHours - info.prevTime) * 60.0);
            }
            return info;
        }
    }

    // After Isha - next prayer is tomorrow's Fajr
    info.nextPrayer = FAJR;
    info.nextTime = GetPrayerTime(schedule, FAJR) + 24.0;
    info.remainingMinutes = static_cast<int>((info.nextTime - currentHours) * 60.0);
    info.prevPrayer = ISHA;
    info.prevTime = GetPrayerTime(schedule, ISHA);
    info.elapsedMinutes = static_cast<int>((currentHours - info.prevTime) * 60.0);
    return info;
}
