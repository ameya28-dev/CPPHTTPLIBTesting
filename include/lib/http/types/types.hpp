#ifndef HTTPLIBTESTING_CONSTRAINT_TYPES_HPP
#define HTTPLIBTESTING_CONSTRAINT_TYPES_HPP

#include <httplib.h>
#include <type_traits>
#include <variant>

#include "lib/http/error/api_error.hpp"
#include <nlohmann/json.hpp>

template <typename T, typename = void>
struct IsJsonDeserializable : std::false_type {};

template <typename T>
struct IsJsonDeserializable<T, std::void_t<decltype(std::declval<nlohmann::json>().get<T>())>> : std::true_type {};

template <typename T, typename = void>
struct IsJsonSerializable : std::false_type {};

template <typename T>
struct IsJsonSerializable<T, std::void_t<decltype(nlohmann::json(std::declval<T>()))>> : std::true_type {};

struct Empty {};

inline void from_json(const nlohmann::json&, Empty) {}

template <typename T>
struct Success {
    int status;
    T data;
};

struct Failure {
    ApiError error;
    int rawStatus;
    std::string message;
    std::string rawBody;
};

template <typename T>
using ApiResult = std::variant<Success<T>, Failure>;

using HttpParams  = httplib::Params;
using HttpHeaders = httplib::Headers;

#endif // HTTPLIBTESTING_CONSTRAINT_TYPES_HPP
