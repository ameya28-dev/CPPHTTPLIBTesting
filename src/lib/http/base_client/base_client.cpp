#include "lib/http/base_client/base_client.hpp"

#include "lib/http/types/types.hpp"



using namespace std::chrono_literals;

BaseClient::BaseClient(const std::string& host) : _client(host), _gen(100, 900) {
    setTimeouts(5s, 5s);
}

auto BaseClient::setVerbose(const bool v) -> void {
    _verbose = v;
}

auto BaseClient::setRetries(const int r) -> void {
    _retryCount = r;
}

auto BaseClient::setTimeouts(const std::chrono::nanoseconds connTimeout, const std::chrono::nanoseconds readTimeout)
    -> void {
    auto split = [](const std::chrono::nanoseconds duration) {
        const auto seconds      = std::chrono::duration_cast<std::chrono::seconds>(duration);
        const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration - seconds);
        return std::make_pair(seconds.count(), static_cast<long>(microseconds.count()));
    };

    auto [connSeconds, connMicroseconds] = split(connTimeout);
    auto [readSeconds, readMicroseconds] = split(readTimeout);
    _client.set_connection_timeout(connSeconds, connMicroseconds);
    _client.set_read_timeout(readSeconds, readMicroseconds);
}

auto BaseClient::setDefaultHeaders(const httplib::Headers& headers) -> void {
    _client.set_default_headers(headers);
}

auto BaseClient::Delete(const std::string& path, const httplib::Headers& headers, const httplib::Params& params)
    -> ApiResult<Empty> {
    return executeWithRetry<Empty>([&](const std::string& p) { return _client.Delete(p, headers, params); }, path);
}

auto BaseClient::getClientOrServerError(int status) -> ApiError {
    const auto it = std::find_if(std::begin(allApiErrors), std::end(allApiErrors),
        [status](const ApiError err) { return static_cast<int>(err) == status; });
    if (it != std::end(allApiErrors)) {
        return static_cast<ApiError>(status);
    }
    if (status < 500) {
        return ApiError::ClientHTTPError;
    }
    return ApiError::ServerHTTPError;
}

auto BaseClient::mapConnectionError(const httplib::Error error) -> ApiError {
    switch (error) {
    case httplib::Error::ConnectionTimeout:
        return ApiError::ConnectionTimeout;
        break;
    case httplib::Error::Timeout:
        return ApiError::Timeout;
        break;
    default:
        return ApiError::NetworkTransportError;
    }
}
