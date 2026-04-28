#ifndef HTTPLIBTESTING_BASE_CLIENT_HPP
#define HTTPLIBTESTING_BASE_CLIENT_HPP

#include <chrono>
#include <fmt/base.h>
#include <fmt/chrono.h>
#include <httplib.h>
#include <string>
#include <thread>
#include <type_traits>

#include "lib/http/types/types.hpp"
#include "lib/http/constants/constants.hpp"
#include "lib/random/random.hpp"
#include "lib/logger/manager/manager.hpp"

class BaseClient {
public:
    explicit BaseClient(const std::string&);

    auto setVerbose(bool) -> void;

    auto setRetries(int) -> void;

    auto setTimeouts(std::chrono::nanoseconds, std::chrono::nanoseconds) -> void;

    auto setDefaultHeaders(const httplib::Headers&) -> void;

    template <typename Resp>
    auto Get(const std::string&, const httplib::Params& = {}, const httplib::Headers& = {})
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value, ApiResult<Resp>>;

    template <typename Resp, typename Req>
    auto Post(const std::string&, const Req&, const httplib::Headers& = {})
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>>;

    template <typename Resp, typename Req>
    auto Put(const std::string&, const Req&, const httplib::Headers& = {})
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>>;

    template <typename Resp, typename Req>
    auto Patch(const std::string&, const Req&, const httplib::Headers& = {})
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>>;

    auto Delete(const std::string&, const httplib::Headers& = {}, const httplib::Params& = {}) -> ApiResult<Empty>;

private:
    template <typename T, typename Func>
    auto executeWithRetry(Func&&, const std::string&) -> ApiResult<T>;

    template <typename T>
    auto parseToVariant(httplib::Result&&) -> ApiResult<T>;

    static auto getClientOrServerError(int status) -> ApiError;

    static auto mapConnectionError(httplib::Error) -> ApiError;

private:
    httplib::Client _client;
    bool _verbose   = ServiceConstants::Defaults::Verbose;
    int _retryCount = ServiceConstants::Defaults::RetryCount;
    UniformInt _gen;
};

template <typename Resp>
auto BaseClient::Get(const std::string& path, const httplib::Params& params, const httplib::Headers& headers)
    -> std::enable_if_t<IsJsonDeserializable<Resp>::value, ApiResult<Resp>> {
    return executeWithRetry<Resp>([&](const std::string& p) { return _client.Get(p, params, headers); }, path);
}

template <typename Resp, typename Req>
auto BaseClient::Post(const std::string& path, const Req& body, const httplib::Headers& headers)
    -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>> {
    const auto payload = nlohmann::to_string(body);
    return executeWithRetry<Resp>(
        [&](const std::string& p) {
            _client.Post(p, headers, payload, ServiceConstants::Defaults::JSONApplicationType);
        },
        path);
}

template <typename Resp, typename Req>
auto BaseClient::Put(const std::string& path, const Req& body, const httplib::Headers& headers)
    -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>> {
    const auto payload = nlohmann::to_string(body);
    return executeWithRetry<Resp>(
        [&](const std::string& p) {
            _client.Put(p, headers, payload, ServiceConstants::Defaults::JSONApplicationType);
        },
        path);
}

template <typename Resp, typename Req>
auto BaseClient::Patch(const std::string& path, const Req& body, const httplib::Headers& headers)
    -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>> {
    const auto payload = nlohmann::to_string(body);
    return executeWithRetry<Resp>(
        [&](const std::string& p) {
            _client.Patch(p, headers, payload, ServiceConstants::Defaults::JSONApplicationType);
        },
        path);
}

template <typename T, typename Func>
ApiResult<T> BaseClient::executeWithRetry(Func&& func, const std::string& path) {
    httplib::Result res{};
    int attempts = 0;

    while (true) {
        res = func(path);
        if (res && res->status < 500) {
            return parseToVariant<T>(std::move(res));
        }

        if (attempts >= _retryCount) {
            auto finalCause = res ? std::to_string(res->status) : httplib::to_string(res.error());
            return Failure{ApiError::MaxRetriesReached, res ? res->status : -1,
                fmt::format("Retry limit ({}) reached. Final cause: {}", _retryCount, finalCause),
                res ? res->body : ""};
        }
        attempts++;
        auto waitDuration = std::chrono::seconds(1 << (attempts - 1)) + std::chrono::microseconds(_gen.next());
        if (_verbose) {
            LOG_INFO("[RETRY] Attempt {}/{} failed. Waiting {}ms...\n", attempts, _retryCount, waitDuration);
        }
        std::this_thread::sleep_for(waitDuration);
    }
}

template <typename T>
ApiResult<T> BaseClient::parseToVariant(httplib::Result&& res) {
    if (!res) {
        const ApiError error = mapConnectionError(res.error());
        return Failure{error, -1, httplib::to_string(res.error()), ""};
    }

    int status = res->status;
    if (status < 200) {
        return Failure{ApiError::Unknown, status, httplib::to_string(res.error()), res->body};
    }

    if (status >= 300 && status < 400) {
        return Failure{ApiError::Redirection, status, httplib::to_string(res.error()), res->body};
    }

    if (status >= 400 && status < 600) {
        const auto error = getClientOrServerError(status);
        return Failure{error, status, "HTTP Error", res->body};
    }

    try {
        if constexpr (std::is_same_v<T, Empty>) {
            return Success<Empty>{status, Empty{}};
        }

        nlohmann::json j = res->body.empty() ? nlohmann::json::object() : nlohmann::json::parse(res->body);
        return Success<T>{status, j.get<T>()};
    } catch (const nlohmann::json::parse_error& ex) {
        return Failure{ApiError::InvalidJson, status, ex.what(), res->body};
    } catch (const std::exception& e) {
        return Failure{ApiError::MappingError, res->status, e.what(), res->body};
    }
}

#endif // HTTPLIBTESTING_BASE_CLIENT_HPP
