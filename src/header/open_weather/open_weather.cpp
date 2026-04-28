#include "header/open_weather/open_weather.hpp"

#include <httplib.h>

#include "header/constants/constants.hpp"

OpenWeather::OpenWeather() : _client(ServiceConstants::Endpoints::Base) {}

std::variant<weather::WeatherData, weather::WeatherError> OpenWeather::getForCity(std::string_view city) {
    _client.setVerbose(true);
    auto params = _params;
    params.emplace(ServiceConstants::Keys::Q, city);

    const auto start = std::chrono::steady_clock::now();
    auto res         = _client.Get<weather::WeatherData>(ServiceConstants::Endpoints::Weather, params);
    const auto end   = std::chrono::steady_clock::now();

    PRINT_INFO("Time elapsed = {}", std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

    if (const auto it = std::get_if<Success<weather::WeatherData>>(&res)) {
        return it->data;
    }
    weather::WeatherError error = nlohmann::json::parse(std::get<Failure>(res).rawBody);
    return error;
}

std::variant<forecast::WeatherForecast, weather::WeatherError> OpenWeather::getForecastForCity(std::string_view city) {
    _client.setVerbose(true);
    auto params = _params;
    params.emplace(ServiceConstants::Keys::Q, city);

    const auto start = std::chrono::steady_clock::now();
    auto res         = _client.Get<forecast::WeatherForecast>(ServiceConstants::Endpoints::Forecast, params);
    const auto end   = std::chrono::steady_clock::now();

    PRINT_INFO("Time elapsed = {}", std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

    if (const auto it = std::get_if<Success<forecast::WeatherForecast>>(&res)) {
        return it->data;
    }
    return nlohmann::json::parse(std::get<Failure>(res).rawBody).get<weather::WeatherError>();
}

httplib::Params OpenWeather::getParams() const {
    return _params;
}
