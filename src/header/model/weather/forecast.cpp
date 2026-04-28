
#include "header/model/weather/forecast/forecast.hpp"

#include <optional>
#include <string>

#include "header/serialization/optional_json.hpp"

namespace forecast {
    void from_json(const nlohmann::json& j, MainClass& main) {
        j.at("temp").get_to(main.temp);
        j.at("feels_like").get_to(main.feelsLike);
        j.at("temp_min").get_to(main.tempMin);
        j.at("temp_max").get_to(main.tempMax);
        j.at("pressure").get_to(main.pressure);
        j.at("sea_level").get_to(main.seaLevel);
        j.at("grnd_level").get_to(main.grndLevel);
        j.at("humidity").get_to(main.humidity);
        j.at("temp_kf").get_to(main.tempKf);
    }

    void to_json(nlohmann::json& j, const MainClass& main) {
        j["temp"]       = main.temp;
        j["feels_like"] = main.feelsLike;
        j["temp_min"]   = main.tempMin;
        j["temp_max"]   = main.tempMax;
        j["pressure"]   = main.pressure;
        j["sea_level"]  = main.seaLevel;
        j["grnd_level"] = main.grndLevel;
        j["humidity"]   = main.humidity;
        j["temp_kf"]    = main.tempKf;
    }

    void from_json(const nlohmann::json& j, Rain& r) {
        j.at("3h").get_to(r.the3H);
    }

    void to_json(nlohmann::json& j, const Rain& r) {
        j["3h"] = r.the3H;
    }

    void from_json(const nlohmann::json& j, Sys& s) {
        auto val = j.at("pod").get<std::string>();

        if (val.empty()) {
            throw std::invalid_argument("Pod string is empty");
        }

        s.pod = toPodEnum(val[0]);
    }

    void to_json(nlohmann::json& j, const Sys& s) {
        j["pod"] = std::to_string(static_cast<char>(s.pod));
    }

    void from_json(const nlohmann::json& j, Weather& w) {
        j.at("id").get_to(w.id);
        w.main        = toMainEnum(j.at("main").get<std::string>());
        w.description = toDescriptionEnum(j.at("description").get<std::string>());
        j.at("icon").get_to(w.icon);
    }

    void to_json(nlohmann::json& j, const Weather& w) {
        j["id"]          = w.id;
        j["main"]        = getMainEnumDesc(w.main);
        j["description"] = getDescriptionEnumDesc(w.description);
        j["icon"]        = w.icon;
    }

    void from_json(const nlohmann::json& j, List& list) {
        j.at("dt").get_to(list.dt);
        j.at("main").get_to(list.main);
        j.at("weather").get_to(list.weather);
        j.at("clouds").get_to(list.clouds);
        j.at("wind").get_to(list.wind);
        j.at("visibility").get_to(list.visibility);
        j.at("pop").get_to(list.pop);
        j.at("sys").get_to(list.sys);
        j.at("dt_txt").get_to(list.dtTxt);
        list.rain = j.value("rain", std::optional<Rain>(std::nullopt));
    }

    void to_json(nlohmann::json& j, const List& list) {
        j["dt"]         = list.dt;
        j["main"]       = list.main;
        j["weather"]    = list.weather;
        j["clouds"]     = list.clouds;
        j["wind"]       = list.wind;
        j["visibility"] = list.visibility;
        j["pop"]        = list.pop;
        j["sys"]        = list.sys;
        j["dt_txt"]     = list.dtTxt;
        if (list.rain) {
            j["rain"] = list.rain;
        }
    }
} // namespace forecast
