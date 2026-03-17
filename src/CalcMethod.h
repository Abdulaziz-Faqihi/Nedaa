#pragma once

enum class CalcMethodId {
    UMM_AL_QURA,   // Saudi, Yemen - Fajr 18.5, Isha 90min after Maghrib
    MWL,            // Muslim World League - Fajr 18, Isha 17
    EGYPTIAN,       // Egyptian General Auth - Fajr 19.5, Isha 17.5
    KARACHI,        // Univ. Islamic Sciences, Karachi - Fajr 18, Isha 18
    ISNA,           // Islamic Society of North America - Fajr 15, Isha 15
    DIYANET,        // Turkey Diyanet - Fajr 18, Isha 17
    TEHRAN,         // Inst. Geophysics Tehran - Fajr 17.7, Isha 14
    SINGAPORE,      // Singapore/Malaysia/Indonesia - Fajr 20, Isha 18
};

struct CalcMethodParams {
    double fajrAngle;
    double ishaAngle;    // 0 if using minutes
    int ishaMinutes;     // 0 if using angle
};

const CalcMethodParams& GetCalcMethod(CalcMethodId id);
CalcMethodId GetCountryCalcMethod(const char* iso2);
