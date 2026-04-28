#include <chrono>
#include <date/date.h>
#include <date/tz.h>
#include <sstream>
#include <stdexcept>
#include <variant>
#include <vector>

#include "header/constants/constants.hpp"
#include "header/date/dob.hpp"
#include "header/open_weather/open_weather.hpp"
#include "lib/logger/manager/manager.hpp"

template <>
struct fmt::formatter<date::zoned_seconds> {
    static constexpr auto parse(const fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    static auto format(const date::zoned_seconds& zt, const fmt::format_context& ctx) {
        return fmt::format_to(ctx.out(), "{}", date::format(ServiceConstants::Pattern::Default::DateTime, zt));
    }
};

void getWeatherForCity(std::string_view city) {
    OpenWeather openWeather;
    if (const auto res = openWeather.getForCity(city); const auto error = std::get_if<weather::WeatherError>(&res)) {
        LOG_ERROR("city: {}, status code: {}, Message: {}", city, error->cod, error->message);
        PRINT_ERROR("city: {}, status code: {}, Message: {}", city, error->cod, error->message);
    } else {
        const auto data = std::get<weather::WeatherData>(res);
        PRINT_INFO("current temperature in celsius: {}", data.main.temp);
        PRINT_INFO("Location => lat: {}, long: {}", data.coord.lat, data.coord.lon);
        PRINT_INFO("description: {}", data.weather[0].description);
        PRINT_INFO("min: {}, max: {}, feels like: {}, humidity: {}", data.main.tempMin, data.main.tempMax,
            data.main.feelsLike, data.main.humidity);
        PRINT_INFO("wind speed: {}, degree: {}", data.wind.speed, data.wind.deg);
        PRINT_INFO(data);
    }
}

date::zoned_seconds getISTDatetimeFromGMTString(const std::string& gmtDate) {
    std::stringstream ss{gmtDate};
    date::sys_seconds gmtTP;
    ss >> date::parse("%F %T", gmtTP);
    if (ss.fail()) {
        LOG_FATAL("Failed to parse string date: {}", gmtDate);
        PRINT_FATAL("Failed to parse string date: {}", gmtDate);
        throw std::invalid_argument("Failed to parse string date: " + gmtDate);
    }

    return date::make_zoned("Asia/Kolkata", gmtTP);
}

date::zoned_seconds getISTDatetimeFromUnix(int64_t epoch) {
    const auto tp = date::sys_seconds{std::chrono::seconds{epoch}};
    return date::make_zoned("Asia/Kolkata", tp);
}

void processForecast(const std::vector<forecast::List>& list, const forecast::City& city) {
    PRINT_INFO("City Details:");
    PRINT_INFO("City: {}, Latitude: {}, Longitude: {}, Population: {}", city.name, city.coord.lat, city.coord.lon,
        city.population);
    PRINT_INFO("Sunrise: {}, Sunset: {}",
        date::format(ServiceConstants::Pattern::Indian::Time, getISTDatetimeFromUnix(city.sunrise)),
        date::format(ServiceConstants::Pattern::Indian::Time, getISTDatetimeFromUnix(city.sunset)));

    PRINT_INFO("Weather forecast on given datetime:");
    PRINT_INFO("| {:^10} | {:^11} | {:^5} | {:^5} | {:^5} | {:^3} | {:^10} | {:^20} |", "Date", "Time", "Min", "Max",
        "Feels", "Hum", "Condition", "Description");
    PRINT_INFO(
        "+{:-<11}-+-{:-<11}-+-{:-<5}-+-{:-<5}-+-{:-<5}-+-{:-<3}-+-{:-<10}-+-{:-<21}+", "", "", "", "", "", "", "", "");
    for (const auto& pred : list) {
        try {
            auto istTime = getISTDatetimeFromGMTString(pred.dtTxt);
            PRINT_INFO("| {:<10} | {:<11} | {:>5.2f} | {:>5.2f} | {:>5.2f} | {:<3} | {:^10} | {:^20} |",
                date::format(ServiceConstants::Pattern::Default::Date, istTime), // Date
                date::format(ServiceConstants::Pattern::Indian::Time, istTime), // Time
                pred.main.tempMin, // Min Temp
                pred.main.tempMax, // Max Temp
                pred.main.feelsLike, // Feels Like
                pred.main.humidity, // Humidity
                forecast::getMainEnumDesc(pred.weather[0].main), // Condition
                forecast::getDescriptionEnumDesc(pred.weather[0].description) // Description
            );
        } catch (...) {
            continue;
        }
    }
}

void getForecastForCity(std::string_view city) {
    OpenWeather openWeather;
    if (const auto res   = openWeather.getForecastForCity(city);
        const auto error = std::get_if<weather::WeatherError>(&res)) {
        LOG_ERROR("city: {}, status code: {}, Message: {}", city, error->cod, error->message);
        PRINT_ERROR("city: {}, status code: {}, Message: {}", city, error->cod, error->message);
    } else {
        const auto forecastResponse = std::get<forecast::WeatherForecast>(res);
        processForecast(forecastResponse.list, forecastResponse.city);
    }
}

void setTimeZoneDB() {
#if WIN32
    const std::filesystem::path path = "resources/tzdata";
    date::set_install(path.string());
    try {
        date::get_tzdb();
    } catch (const std::runtime_error& ex) {
        LOG_FATAL("Error loading bundled TZDB: {}", ex.what());
        std::terminate();
    }
#endif
}

int main() {
    LOG_INIT();
    setTimeZoneDB();
    checkSizeAndAlignmentOfDate();
    dateCurrentAge();

    getWeatherForCity(ServiceConstants::Weather::Hubli);
    getWeatherForCity("ServiceConstants::Weather::Hubli");
    getWeatherForCity("");

    getForecastForCity(ServiceConstants::Weather::Hubli);
}
