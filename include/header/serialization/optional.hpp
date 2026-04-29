#ifndef HTTPLIBTESTING_INCLUDE_HEADER_SERIALIZATION_FORECAST_HPP
#define HTTPLIBTESTING_INCLUDE_HEADER_SERIALIZATION_FORECAST_HPP

#include <optional>

#include <nlohmann/json.hpp>

template <typename T>
struct nlohmann::adl_serializer<std::optional<T>> {
    static void from_json(const json& json, std::optional<T>& opt) {
        if (json.is_null()) {
            opt = std::nullopt;
        } else {
            opt = json.get<T>();
        }
    }

    static void to_json(json& json, const std::optional<T>& opt) {
        if (opt == std::nullopt) {
            json = nullptr;
        } else {
            json = *opt;
        }
    }
}; // namespace nlohmann

#endif
