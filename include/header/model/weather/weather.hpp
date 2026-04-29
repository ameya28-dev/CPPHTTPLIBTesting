#ifndef HTTPLIBTESTING_WEATHER_HPP
#define HTTPLIBTESTING_WEATHER_HPP

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

#include "header/serialization/optional.hpp"
#include <nlohmann/json.hpp>

namespace weather {
    struct Clouds {
        int32_t all;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Clouds, all)
    };

    struct Coord {
        double lon;
        double lat;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Coord, lon, lat)
    };

    struct MainClass {
        double feelsLike;
        double temp;
        double tempMin;
        double tempMax;
        int32_t pressure;
        int32_t humidity;
        int32_t seaLevel;
        int32_t grndLevel;
    };

    void from_json(const nlohmann::json&, MainClass&);

    void to_json(nlohmann::json&, const MainClass&);

    struct Sys {
        int32_t id;
        std::string country;
        int32_t sunrise;
        int32_t sunset;
        std::optional<int32_t> type;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Sys, id, country, sunrise, sunset, type)
    };

    struct Weather {
        int32_t id;
        std::string main;
        std::string description;
        std::string icon;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Weather, id, main, description, icon)
    };

    struct Wind {
        double speed;
        int32_t deg;
        std::optional<double> gust;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Wind, speed, deg, gust)
    };

    struct WeatherData {
        Coord coord;
        std::vector<Weather> weather;
        std::string base;
        MainClass main;
        int32_t visibility;
        Wind wind;
        Clouds clouds;
        int32_t dt;
        Sys sys;
        int32_t timezone;
        int32_t id;
        std::string name;
        int32_t cod;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
            WeatherData, coord, weather, base, main, visibility, wind, clouds, dt, sys, timezone, id, name, cod)
    };
} // namespace weather

template <>
struct fmt::formatter<weather::Clouds> : formatter<std::string> {
    constexpr auto parse(format_parse_context& context) {
        return formatter<std::string>::parse(context);
    }

    static auto format(const weather::Clouds& clouds, const format_context& context) {
        return format_to(context.out(), "Clouds(all: {})", clouds.all);
    }
};

template <>
struct fmt::formatter<weather::Coord> : formatter<std::string> {
    constexpr auto parse(format_parse_context& context) {
        return formatter<std::string>::parse(context);
    }

    static auto format(const weather::Coord& coord, const format_context& context) {
        return format_to(context.out(), "Coord(lon: {}, lat: {})", coord.lon, coord.lat);
    }
};

template <>
struct fmt::formatter<weather::MainClass> : formatter<std::string> {
    constexpr auto parse(format_parse_context& context) {
        return formatter<std::string>::parse(context);
    }

    static auto format(const weather::MainClass& main, const format_context& context) {
        return format_to(context.out(),
            "Main(temp: {}, feels_like: {}, temp_min: {}, temp_max: {}, pressure: {}, humidity: {}, sea_level: {}, "
            "grnd_level: {})",
            main.temp, main.feelsLike, main.tempMin, main.tempMax, main.pressure, main.humidity, main.seaLevel,
            main.grndLevel);
    }
};

template <>
struct fmt::formatter<weather::Sys> : formatter<std::string> {
    constexpr auto parse(format_parse_context& context) {
        return formatter<std::string>::parse(context);
    }

    static auto format(const weather::Sys& sys, const format_context& context) {
        return format_to(context.out(), "Sys(id: {}, country: {}, sunrise: {}, sunset: {}, type: {})", sys.id,
            sys.country, sys.sunrise, sys.sunset, sys.type);
    }
};

template <>
struct fmt::formatter<weather::Weather> : formatter<std::string> {
    constexpr auto parse(format_parse_context& context) {
        return formatter<std::string>::parse(context);
    }

    static auto format(const weather::Weather& weather, const format_context& context) {
        return format_to(context.out(), "Weather(id: {}, main: {}, description: {}, icon: {})", weather.id,
            weather.main, weather.description, weather.icon);
    }
};

template <>
struct fmt::formatter<weather::Wind> : formatter<std::string> {
    constexpr auto parse(format_parse_context& context) {
        return formatter<std::string>::parse(context);
    }

    static auto format(const weather::Wind& wind, const format_context& context) {
        return format_to(context.out(), "Wind(speed: {}, deg: {}, gust: {})", wind.speed, wind.deg, wind.gust);
    }
};

template <>
struct fmt::formatter<weather::WeatherData> : formatter<std::string> {
    constexpr auto parse(format_parse_context& context) {
        return formatter<std::string>::parse(context);
    }

    static auto format(const weather::WeatherData& data, const format_context& context) {
        return format_to(context.out(),
            "WeatherData(coord: {}, weather: {}, base: {}, main: {}, visibility: {}, wind: {}, clouds: {}, dt: {}, "
            "sys: {}, timezone: {}, id: {}, name: {}, cod: {})",
            data.coord, data.weather, data.base, data.main, data.visibility, data.wind, data.clouds, data.dt, data.sys,
            data.timezone, data.id, data.name, data.cod);
    }
};

#endif // HTTPLIBTESTING_WEATHER_HPP
