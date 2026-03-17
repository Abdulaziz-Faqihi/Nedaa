#include "Cities.h"

#include <windows.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>
#include <string>

using json = nlohmann::json;

// Convert a UTF-8 string to a wide string using Windows API
static std::wstring Utf8ToWide(const std::string& utf8)
{
    if (utf8.empty())
        return {};
    int needed = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    if (needed <= 0)
        return {};
    std::wstring result(static_cast<size_t>(needed), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), result.data(), needed);
    return result;
}

CitiesDatabase& CitiesDatabase::Instance()
{
    static CitiesDatabase instance;
    return instance;
}

bool CitiesDatabase::LoadWorldData(const std::wstring& dataDir)
{
    // --- Load countries.json ---
    {
        std::wstring path = dataDir + L"countries.json";
        std::ifstream ifs(path);
        if (!ifs.is_open())
            return false;

        json jArr;
        try {
            ifs >> jArr;
        }
        catch (...) {
            return false;
        }

        m_countries.clear();
        m_countries.reserve(jArr.size());

        for (auto& jObj : jArr) {
            CountryInfo c{};
            c.id = jObj.value("id", 0);
            c.iso2 = jObj.value("iso2", "");
            c.nameEn = Utf8ToWide(jObj.value("name_en", ""));
            c.nameAr = Utf8ToWide(jObj.value("name_ar", ""));

            // gmt_offset is in seconds, convert to hours
            double gmtOffsetSec = jObj.value("gmt_offset", 0.0);
            c.timezoneHours = gmtOffsetSec / 3600.0;

            c.calcMethod = GetCountryCalcMethod(c.iso2.c_str());

            m_countries.push_back(std::move(c));
        }

        // Sort countries by Arabic name
        std::sort(m_countries.begin(), m_countries.end(),
            [](const CountryInfo& a, const CountryInfo& b) {
                return a.nameAr < b.nameAr;
            });
    }

    // --- Load states.json ---
    {
        std::wstring path = dataDir + L"states.json";
        std::ifstream ifs(path);
        if (!ifs.is_open())
            return false;

        json jArr;
        try {
            ifs >> jArr;
        }
        catch (...) {
            return false;
        }

        m_states.clear();
        m_states.reserve(jArr.size());

        for (auto& jObj : jArr) {
            StateInfo s{};
            s.id = jObj.value("id", 0);
            s.countryId = jObj.value("country_id", 0);
            s.nameEn = Utf8ToWide(jObj.value("name_en", ""));
            s.nameAr = Utf8ToWide(jObj.value("name_ar", ""));

            // lat and lng come as strings
            std::string latStr = jObj.value("lat", "0");
            std::string lngStr = jObj.value("lng", "0");
            try { s.latitude = std::stod(latStr); } catch (...) { s.latitude = 0.0; }
            try { s.longitude = std::stod(lngStr); } catch (...) { s.longitude = 0.0; }

            m_states.push_back(std::move(s));
        }

        // Sort states by countryId, then by Arabic name
        std::sort(m_states.begin(), m_states.end(),
            [](const StateInfo& a, const StateInfo& b) {
                if (a.countryId != b.countryId)
                    return a.countryId < b.countryId;
                return a.nameAr < b.nameAr;
            });
    }

    return true;
}

bool CitiesDatabase::LoadSaudiData(const std::wstring& dataDir)
{
    // --- Load sa_regions_lite.json ---
    {
        std::wstring path = dataDir + L"sa_regions_lite.json";
        std::ifstream ifs(path);
        if (!ifs.is_open())
            return false;

        json jArr;
        try {
            ifs >> jArr;
        }
        catch (...) {
            return false;
        }

        m_saudiRegions.clear();
        m_saudiRegions.reserve(jArr.size());

        for (auto& jObj : jArr) {
            RegionInfo r{};
            r.regionId = jObj.value("region_id", 0);
            r.nameAr = Utf8ToWide(jObj.value("name_ar", ""));
            r.nameEn = Utf8ToWide(jObj.value("name_en", ""));
            m_saudiRegions.push_back(std::move(r));
        }

        std::sort(m_saudiRegions.begin(), m_saudiRegions.end(),
            [](const RegionInfo& a, const RegionInfo& b) {
                return a.regionId < b.regionId;
            });
    }

    // --- Load sa_cities.json ---
    {
        std::wstring path = dataDir + L"sa_cities.json";
        std::ifstream ifs(path);
        if (!ifs.is_open())
            return false;

        json jArr;
        try {
            ifs >> jArr;
        }
        catch (...) {
            return false;
        }

        m_saudiCities.clear();
        m_saudiCities.reserve(jArr.size());

        for (auto& jObj : jArr) {
            CityInfo c{};
            c.cityId = jObj.value("city_id", 0);
            c.regionId = jObj.value("region_id", 0);
            c.nameAr = Utf8ToWide(jObj.value("name_ar", ""));
            c.nameEn = Utf8ToWide(jObj.value("name_en", ""));

            // center is an array [lat, lng]
            if (jObj.contains("center") && jObj["center"].is_array() && jObj["center"].size() >= 2) {
                c.latitude = jObj["center"][0].get<double>();
                c.longitude = jObj["center"][1].get<double>();
            }
            else {
                c.latitude = 0.0;
                c.longitude = 0.0;
            }

            m_saudiCities.push_back(std::move(c));
        }

        // Sort by regionId then Arabic name
        std::sort(m_saudiCities.begin(), m_saudiCities.end(),
            [](const CityInfo& a, const CityInfo& b) {
                if (a.regionId != b.regionId)
                    return a.regionId < b.regionId;
                return a.nameAr < b.nameAr;
            });
    }

    m_saudiLoaded = true;
    return true;
}

const std::vector<CountryInfo>& CitiesDatabase::GetCountries() const
{
    return m_countries;
}

std::vector<const StateInfo*> CitiesDatabase::GetStatesByCountry(int countryId) const
{
    std::vector<const StateInfo*> result;
    for (const auto& s : m_states) {
        if (s.countryId == countryId)
            result.push_back(&s);
    }
    return result;
}

bool CitiesDatabase::IsSaudiDataLoaded() const
{
    return m_saudiLoaded;
}

const std::vector<RegionInfo>& CitiesDatabase::GetSaudiRegions() const
{
    return m_saudiRegions;
}

std::vector<const CityInfo*> CitiesDatabase::GetSaudiCitiesByRegion(int regionId) const
{
    std::vector<const CityInfo*> result;
    for (const auto& c : m_saudiCities) {
        if (c.regionId == regionId)
            result.push_back(&c);
    }
    return result;
}

const CityInfo* CitiesDatabase::FindSaudiCityById(int cityId) const
{
    for (const auto& c : m_saudiCities) {
        if (c.cityId == cityId)
            return &c;
    }
    return nullptr;
}

const CountryInfo* CitiesDatabase::FindCountryById(int countryId) const
{
    for (const auto& c : m_countries) {
        if (c.id == countryId)
            return &c;
    }
    return nullptr;
}

const CountryInfo* CitiesDatabase::FindCountryByIso2(const std::string& iso2) const
{
    for (const auto& c : m_countries) {
        if (c.iso2 == iso2)
            return &c;
    }
    return nullptr;
}

const StateInfo* CitiesDatabase::FindStateById(int stateId) const
{
    for (const auto& s : m_states) {
        if (s.id == stateId)
            return &s;
    }
    return nullptr;
}
