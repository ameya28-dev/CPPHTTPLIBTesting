#ifndef HTTPLIBTESTING_HEADER_CONSTANTS_HPP
#define HTTPLIBTESTING_HEADER_CONSTANTS_HPP

namespace ServiceConstants {
    struct Endpoints {
        static constexpr auto Base     = "https://api.openweathermap.org";
        static constexpr auto Weather  = "/data/2.5/weather";
        static constexpr auto Forecast = "/data/2.5/forecast";
    };

    struct Weather {
        static constexpr auto AppId  = "4af409a4c67493e64a7c44c96d9c51e3";
        static constexpr auto Hubli  = "hubli";
        static constexpr auto Metric = "metric";
    };

    struct Keys {
        static constexpr auto AppId = "APPID";
        static constexpr auto Q     = "q";
        static constexpr auto Units = "units";
    };

    struct Pattern {
        struct Default {
            static constexpr auto DateTime = "%d-%m-%Y %H:%M:%S";
            static constexpr auto Date     = "%d-%m-%Y";
            static constexpr auto Time     = "%H:%M:%S";
        };

        struct Indian {
            static constexpr auto Time = "%I:%M:%S %p";
        };
    };

    struct Resources {
        static constexpr auto TZDataPath = "resources/tzdata";
    };
} // namespace ServiceConstants

#endif // HTTPLIBTESTING_HEADER_CONSTANTS_HPP
