#include "CalcMethod.h"
#include <cstring>
#include <array>

static const std::array<CalcMethodParams, 8> s_methods = {{
    { 18.5,  0.0, 90 },  // UMM_AL_QURA: Fajr 18.5, Isha 90min
    { 18.0, 17.0,  0 },  // MWL
    { 19.5, 17.5,  0 },  // EGYPTIAN
    { 18.0, 18.0,  0 },  // KARACHI
    { 15.0, 15.0,  0 },  // ISNA
    { 18.0, 17.0,  0 },  // DIYANET
    { 17.7, 14.0,  0 },  // TEHRAN
    { 20.0, 18.0,  0 },  // SINGAPORE
}};

const CalcMethodParams& GetCalcMethod(CalcMethodId id)
{
    return s_methods[static_cast<int>(id)];
}

struct CountryMethodEntry {
    const char* iso2;
    CalcMethodId method;
};

static const CountryMethodEntry s_countryMethods[] = {
    // Umm Al-Qura
    { "SA", CalcMethodId::UMM_AL_QURA },
    { "YE", CalcMethodId::UMM_AL_QURA },
    // Egyptian
    { "EG", CalcMethodId::EGYPTIAN },
    { "SD", CalcMethodId::EGYPTIAN },
    { "LY", CalcMethodId::EGYPTIAN },
    // Diyanet
    { "TR", CalcMethodId::DIYANET },
    // Tehran
    { "IR", CalcMethodId::TEHRAN },
    // Karachi
    { "PK", CalcMethodId::KARACHI },
    { "IN", CalcMethodId::KARACHI },
    { "BD", CalcMethodId::KARACHI },
    { "AF", CalcMethodId::KARACHI },
    // Singapore
    { "ID", CalcMethodId::SINGAPORE },
    { "MY", CalcMethodId::SINGAPORE },
    { "SG", CalcMethodId::SINGAPORE },
    { "BN", CalcMethodId::SINGAPORE },
    { "TH", CalcMethodId::SINGAPORE },
    // ISNA
    { "US", CalcMethodId::ISNA },
    { "CA", CalcMethodId::ISNA },
};

CalcMethodId GetCountryCalcMethod(const char* iso2)
{
    for (const auto& entry : s_countryMethods) {
        if (std::strcmp(entry.iso2, iso2) == 0)
            return entry.method;
    }
    return CalcMethodId::MWL;
}
