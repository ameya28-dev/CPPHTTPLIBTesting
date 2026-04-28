#ifndef HTTPLIBTESTING_INCLUDE_HEADER_MODEL_WEATHER_FORECAST_ENUM_HPP
#define HTTPLIBTESTING_INCLUDE_HEADER_MODEL_WEATHER_FORECAST_ENUM_HPP

#include <stdexcept>

namespace forecast {

#define POD_ENUM(X) X(d) X(n)

    enum struct Pod : char {
#define AS_POD_NAME(name) name,
        POD_ENUM(AS_POD_NAME)
#undef AS_POD_NAME
    };

    constexpr Pod toPodEnum(const char c) {
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

    constexpr Description toDescriptionEnum(const std::string_view str) {
#define AS_DESCRIPTION_LOOKUP(name, desc) \
    if (str == desc)                      \
        return Description::name;
        DESCRIPTION_ENUM(AS_DESCRIPTION_LOOKUP)
#undef AS_DESCRIPTION_LOOKUP
        throw std::invalid_argument("Not a valid string representation of a Description enum");
    }

    constexpr const char* getDescriptionEnumDesc(const Description d) {
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

    constexpr MainEnum toMainEnum(const std::string_view str) {
#define AS_MAIN_ENUM_LOOKUP(name, desc) \
    if (str == desc)                    \
        return MainEnum::name;
        MAIN_ENUM(AS_MAIN_ENUM_LOOKUP)
#undef AS_MAIN_ENUM_LOOKUP
        throw std::invalid_argument("Not a valid string representation of a Main enum");
    }

    constexpr const char* getMainEnumDesc(const MainEnum m) {
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

} // namespace forecast

#endif // HTTPLIBTESTING_INCLUDE_HEADER_MODEL_WEATHER_FORECAST_ENUM_HPP
