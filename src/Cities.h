#pragma once
#include <string>
#include <vector>
#include "CalcMethod.h"

struct CountryInfo {
    int id;
    std::string iso2;
    std::wstring nameAr;
    std::wstring nameEn;
    double timezoneHours;  // GMT offset in hours
    CalcMethodId calcMethod;
};

struct StateInfo {
    int id;
    int countryId;
    std::wstring nameAr;
    std::wstring nameEn;
    double latitude;
    double longitude;
};

struct CityInfo {
    int cityId;
    int regionId;
    std::wstring nameAr;
    std::wstring nameEn;
    double latitude;
    double longitude;
};

struct RegionInfo {
    int regionId;
    std::wstring nameAr;
    std::wstring nameEn;
};

class CitiesDatabase {
public:
    static CitiesDatabase& Instance();

    bool LoadWorldData(const std::wstring& dataDir);
    bool LoadSaudiData(const std::wstring& dataDir);

    const std::vector<CountryInfo>& GetCountries() const;
    std::vector<const StateInfo*> GetStatesByCountry(int countryId) const;

    // Saudi-specific
    static constexpr int SAUDI_COUNTRY_ID = 194;  // dr5hn ID for Saudi Arabia
    bool IsSaudiDataLoaded() const;
    const std::vector<RegionInfo>& GetSaudiRegions() const;
    std::vector<const CityInfo*> GetSaudiCitiesByRegion(int regionId) const;
    const CityInfo* FindSaudiCityById(int cityId) const;

    const CountryInfo* FindCountryById(int countryId) const;
    const CountryInfo* FindCountryByIso2(const std::string& iso2) const;
    const StateInfo* FindStateById(int stateId) const;

    int GetDefaultCityId() const { return 6; } // Makkah

private:
    CitiesDatabase() = default;
    std::vector<CountryInfo> m_countries;
    std::vector<StateInfo> m_states;
    std::vector<RegionInfo> m_saudiRegions;
    std::vector<CityInfo> m_saudiCities;
    bool m_saudiLoaded = false;
};
