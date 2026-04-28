#ifndef HTTPLIBTESTING_INCLUDE_HEADER_MODEL_FORECAST_HPP
#define HTTPLIBTESTING_INCLUDE_HEADER_MODEL_FORECAST_HPP

#include <cstdint>
#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>
#include <optional>
#include <string>
#include <vector>

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

#define POD_ENUM(X) X(d) X(n)

    enum struct Pod : char {
#define AS_POD_NAME(name) name,
        POD_ENUM(AS_POD_NAME)
#undef AS_POD_NAME
    };

    inline constexpr Pod toPodEnum(const char c) {
        switch (c) {
#define AS_POD_DESCRIPTION_LOOKUP(name) \
    case #name[0]:                      \
        return Pod::name;
            POD_ENUM(AS_POD_DESCRIPTION_LOOKUP)
#undef AS_POD_DESCRIPTION_LOOKUP
        default:
            throw std::invalid_argument("Invalid char found for Pod");
        }
    }

    struct Sys {
        Pod pod;
    };

    void from_json(const nlohmann::json& j, Sys& s);
    void to_json(nlohmann::json& j, const Sys& s);

#define DESCRIPTION_ENUM(X)               \
    X(broken_clouds, "broken clouds")     \
    X(clear_sky, "clear sky")             \
    X(few_clouds, "few clouds")           \
    X(light_rain, "light rain")           \
    X(overcast_clouds, "overcast clouds") \
    X(scattered_clouds, "scattered clouds")

    enum struct Description : int {
#define AS_DESCRIPTION_ENUM(name, desc) name,
        DESCRIPTION_ENUM(AS_DESCRIPTION_ENUM)
#undef AS_DESCRIPTION_ENUM
    };

    inline constexpr Description toDescriptionEnum(std::string_view str) {
#define AS_DESCRIPTION_LOOKUP(name, desc) \
    if (str == desc)                      \
        return Description::name;
        DESCRIPTION_ENUM(AS_DESCRIPTION_LOOKUP)
#undef AS_DESCRIPTION_LOOKUP
        throw std::invalid_argument("Not a valid string representation of a Description enum");
    }

    inline constexpr const char* getDescriptionEnumDesc(Description d) {
        switch (d) {
#define AS_DESCRIPTION(name, desc) \
    case Description::name:        \
        return desc;
            DESCRIPTION_ENUM(AS_DESCRIPTION)
#undef AS_DESCRIPTION
        default:
            return "";
        }
    }

#define MAIN_ENUM(X)                \
    X(clear, "Clear")               \
    X(clouds, "Clouds")             \
    X(rain, "Rain")                 \
    X(thunderstorm, "Thunderstorm") \
    X(drizzle, "Drizzle")           \
    X(snow, "Snow")                 \
    X(mist, "Mist")                 \
    X(smoke, "Smoke")               \
    X(haze, "Haze")                 \
    X(sand, "Sand")                 \
    X(ash, "Ash")                   \
    X(squall, "Squall")             \
    X(tornado, "Tornado")

    enum struct MainEnum : int {
#define AS_MAIN_ENUM(name, desc) name,
        MAIN_ENUM(AS_MAIN_ENUM)
#undef AS_MAIN_ENUM
    };

    inline constexpr MainEnum toMainEnum(std::string_view str) {
#define AS_MAIN_ENUM_LOOKUP(name, desc) \
    if (str == desc)                    \
        return MainEnum::name;
        MAIN_ENUM(AS_MAIN_ENUM_LOOKUP)
#undef AS_MAIN_ENUM_LOOKUP
        throw std::invalid_argument("Not a valid string representation of a Main enum");
    }

    inline constexpr const char* getMainEnumDesc(MainEnum m) {
        switch (m) {
#define AS_MAIN_ENUM_DESC(name, desc) \
    case MainEnum::name:              \
        return desc;
            MAIN_ENUM(AS_MAIN_ENUM_DESC)
#undef AS_MAIN_ENUM_DESC
        default:
            return "";
        }
    }

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

#endif
