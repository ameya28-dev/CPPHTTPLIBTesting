#ifndef HTTPLIBTESTING_OPEN_WEATHER_HPP
#define HTTPLIBTESTING_OPEN_WEATHER_HPP

#include <string_view>
#include <variant>

#include "header/constants/constants.hpp"
#include "header/model/weather/error.hpp"
#include "header/model/weather/forecast/forecast.hpp"
#include "header/model/weather/weather.hpp"
#include "lib/http/base_client/base_client.hpp"

class OpenWeather {
public:
    OpenWeather();

    [[nodiscard]] std::variant<weather::WeatherData, weather::WeatherError> getForCity(std::string_view);

    [[nodiscard]] std::variant<forecast::WeatherForecast, weather::WeatherError> getForecastForCity(std::string_view);

    [[nodiscard]] HttpParams getParams() const;

private:
    BaseClient _client;
    const HttpParams _params = {
        {ServiceConstants::Keys::AppId, ServiceConstants::Weather::AppId},
        {ServiceConstants::Keys::Units, ServiceConstants::Weather::Metric},
    };
};

#endif // HTTPLIBTESTING_OPEN_WEATHER_HPP
