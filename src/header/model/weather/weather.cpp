#include "header/model/weather/weather.hpp"

void weather::from_json(const nlohmann::json& j, weather::MainClass& main) {
    j.at("feels_like").get_to(main.feelsLike);
    j.at("temp").get_to(main.temp);
    j.at("temp_min").get_to(main.tempMin);
    j.at("temp_max").get_to(main.tempMax);
    j.at("pressure").get_to(main.pressure);
    j.at("humidity").get_to(main.humidity);
    j.at("sea_level").get_to(main.seaLevel);
    j.at("grnd_level").get_to(main.grndLevel);
}

void weather::to_json(nlohmann::json& j, const weather::MainClass& main) {
    j["feels_like"] = main.feelsLike;
    j["temp"]       = main.temp;
    j["temp_min"]   = main.tempMin;
    j["temp_max"]   = main.tempMax;
    j["pressure"]   = main.pressure;
    j["humidity"]   = main.humidity;
    j["sea_level"]  = main.seaLevel;
    j["grnd_level"] = main.grndLevel;
}
