#ifndef HTTPLIBTESTING_INCLUDE_HEADER_MODEL_FORECAST_HPP
#define HTTPLIBTESTING_INCLUDE_HEADER_MODEL_FORECAST_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "enum.hpp"
#include <nlohmann/json.hpp>

namespace forecast {
    struct Coord {
        double lat;
        double lon;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Coord, lat, lon)
    };

    struct City {
        std::string name;
        std::string country;
        Coord coord;
        int64_t id;
        int64_t population;
        int64_t timezone;
        int64_t sunrise;
        int64_t sunset;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
            City, id, name, coord, country, population, timezone, sunrise, sunset)
    };

    struct Clouds {
        int64_t all;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Clouds, all)
    };

    struct MainClass {
        double temp;
        double feelsLike;
        double tempMin;
        double tempMax;
        int64_t pressure;
        int64_t seaLevel;
        int64_t grndLevel;
        int64_t humidity;
        double tempKf;
    };

    void from_json(const nlohmann::json& j, MainClass& main);
    void to_json(nlohmann::json& j, const MainClass& main);

    struct Rain {
        double the3H;
    };

    void from_json(const nlohmann::json& j, Rain& r);
    void to_json(nlohmann::json& j, const Rain& r);

    struct Sys {
        Pod pod;
    };

    void from_json(const nlohmann::json& j, Sys& s);
    void to_json(nlohmann::json& j, const Sys& s);

    struct Weather {
        std::string icon;
        int64_t id;
        MainEnum main;
        Description description;
    };

    void from_json(const nlohmann::json& j, Weather& w);
    void to_json(nlohmann::json& j, const Weather& w);

    struct Wind {
        double speed;
        int64_t deg;
        double gust;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Wind, speed, deg, gust)
    };

    struct List {
        int64_t dt;
        MainClass main;
        std::vector<Weather> weather;
        Clouds clouds;
        Wind wind;
        int64_t visibility;
        double pop;
        Sys sys;
        std::string dtTxt;
        std::optional<Rain> rain;
    };

    void from_json(const nlohmann::json& j, List& list);
    void to_json(nlohmann::json& j, const List& list);

    struct WeatherForecast {
        std::vector<List> list;
        City city;
        std::string cod;
        int64_t message;
        int64_t cnt;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(WeatherForecast, cod, message, cnt, list, city)
    };

} // namespace forecast

#endif
