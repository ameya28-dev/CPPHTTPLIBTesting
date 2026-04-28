#ifndef HTTPLIBTESTING_INCLUDE_HEADER_MODEL_WEATHER_FORECAST_FMT_HPP
#define HTTPLIBTESTING_INCLUDE_HEADER_MODEL_WEATHER_FORECAST_FMT_HPP

#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

namespace forecast {
    struct WeatherForecast;
    struct List;
    struct Wind;
    struct Weather;
    struct Sys;
    struct Rain;
    struct MainClass;
    struct Clouds;
    struct City;
    struct Coord;
} // namespace forecast

template <>
struct fmt::formatter<forecast::Coord> {
    static constexpr auto parse(const fmt::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const forecast::Coord& coord, const fmt::format_context& context) {
        return fmt::format_to(context.out(), "Coord(lat: {}, lon: {})", coord.lat, coord.lon);
    }
};

template <>
struct fmt::formatter<forecast::City> {
    static constexpr auto parse(const fmt::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const forecast::City& city, const fmt::format_context& context) {
        return fmt::format_to(context.out(),
            "City(id: {},name: {},coord: {},country: {},population: {},timezone: {},sunrise: {},sunset: {})", city.id,
            city.name, city.coord, city.country, city.population, city.timezone, city.sunrise, city.sunset);
    }
};

template <>
struct fmt::formatter<forecast::Clouds> {
    static constexpr auto parse(const fmt::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const forecast::Clouds& clouds, const fmt::format_context& context) {
        return fmt::format_to(context.out(), "Clouds(all: {})", clouds.all);
    }
};

template <>
struct fmt::formatter<forecast::MainClass> {
    static constexpr auto parse(const fmt::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const forecast::MainClass& main, const fmt::format_context& context) {
        return fmt::format_to(context.out(),
            "Main(temp: {},feelsLike: {},tempMin: {},tempMax: {},pressure: {},seaLevel: {},grndLevel: {},humidity: "
            "{},tempKf: {})",
            main.temp, main.feelsLike, main.tempMin, main.tempMax, main.pressure, main.seaLevel, main.grndLevel,
            main.humidity, main.tempKf);
    }
};

template <>
struct fmt::formatter<forecast::Rain> {
    static constexpr auto parse(const fmt::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const forecast::Rain& rain, const fmt::format_context& context) {
        return fmt::format_to(context.out(), "Rain(3h: {})", rain.the3H);
    }
};

template <>
struct fmt::formatter<forecast::Sys> {
    static constexpr auto parse(const fmt::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const forecast::Sys& sys, const fmt::format_context& context) {
        return fmt::format_to(context.out(), "Sys(pod: {})", static_cast<char>(sys.pod));
    }
};

template <>
struct fmt::formatter<forecast::Weather> {
    static constexpr auto parse(const fmt::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const forecast::Weather& weather, const fmt::format_context& context) {
        return fmt::format_to(context.out(), "Weather(id: {}, main: {}, description: {}, icon: {})", weather.id,
            getMainEnumDesc(weather.main), getDescriptionEnumDesc(weather.description), weather.icon);
    }
};

template <>
struct fmt::formatter<forecast::Wind> {
    static constexpr auto parse(const fmt::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const forecast::Wind& wind, const fmt::format_context& context) {
        return fmt::format_to(context.out(), "Wind(speed: {}, deg: {}, gust: {})", wind.speed, wind.deg, wind.gust);
    }
};

template <>
struct fmt::formatter<forecast::List> {
    static constexpr auto parse(const fmt::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const forecast::List& list, const fmt::format_context& context) {
        return fmt::format_to(context.out(),
            "List(dt: {}, main: {}, weather: {}, clouds: {}, wind: {}, visibility: {}, pop: {}, sys: {}, dtTxt: "
            "{}, "
            "rain: {})",
            list.dt, list.main, list.weather, list.clouds, list.wind, list.visibility, list.pop, list.sys, list.dtTxt,
            list.rain);
    }
};

template <>
struct fmt::formatter<forecast::WeatherForecast> {
    static constexpr auto parse(const fmt::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const forecast::WeatherForecast& forecast, const fmt::format_context& context) {
        return fmt::format_to(context.out(), "WeatherForecast(cod: {}, message: {}, cnt: {}, list: {}, city: {})",
            forecast.cod, forecast.message, forecast.cnt, forecast.list, forecast.city);
    }
};


#endif // HTTPLIBTESTING_INCLUDE_HEADER_MODEL_WEATHER_FORECAST_FMT_HPP
