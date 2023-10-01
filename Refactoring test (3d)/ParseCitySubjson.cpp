class CityConstruct
{
public:
    string city_name_;
    string city_iso_code_;
    string phone_code_;
    Country country_;

    CityConstruct& SetCityName(std::string city_name) {
        city_name_ = city_name;
        return *this;
    }

    CityConstruct& SetCityIsoCode(std::string city_iso_code) {
        city_iso_code_ = city_iso_code;
        return *this;
    }

    CityConstruct& SetPhoneCode(std::string phone_code) {
        phone_code_ = phone_code;
        return *this;
    }

    CityConstruct& SetCountry(Country country) {
        country_ = move(country);
        return *this;
    }

    City& ConstructCity() {
        return { city_name_, city_iso_code_, phone_code_, country_};
    }
};


// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const Json& json, Country& country)
{
    for (const auto& city_json : json.AsList())
    {
        const auto& city_obj = city_json.AsObject();

        //теперь обьект city конструируется вызовом сеттеров
        City city {SetCityName(city_obj["name"s].AsString()).
                SetCityIsoCode(city_obj["iso_code"s].AsString()).
                SetPhoneCode(country.phone_code + city_obj["phone_code"s].AsString()).
                SetCountry(country).
                ConstructCity()};

        cities.push_back(city);
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
                                    country_obj["name"s].AsString(),
                                    country_obj["iso_code"s].AsString(),
                                    country_obj["phone_code"s].AsString(),
                                    country_obj["time_zone"s].AsString(),
                            });
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        ParseCitySubjson(cities, country_obj["cities"s], country);
    }
}