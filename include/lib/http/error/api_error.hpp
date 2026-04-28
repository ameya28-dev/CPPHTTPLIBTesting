#ifndef HTTPLIBTESTING_API_ERROR_HPP
#define HTTPLIBTESTING_API_ERROR_HPP

#include <array>

#define API_ERROR_LIST(X)                                           \
    X(OK, = 200, "Request Successful")                              \
    X(Created, , "Resource Created")                                \
    X(Accepted, , "Request Accepted For Processing")                \
    X(NoContent, = 204, "Success - No Content Returned")            \
    X(Redirection, = 309, "Redirection Required")                   \
    X(BadRequest, = 400, "Invalid Request Parameter")               \
    X(Unauthorized, , "Authentication Required")                    \
    X(Forbidden, = 403, "Access Denied")                            \
    X(NotFound, , "Resource Not Found")                             \
    X(MethodNotAllowed, , "Request Method Not Supported")           \
    X(RequestTimeout, = 408, "Server Timed Out Waiting")            \
    X(Conflict, , "Conflict Occured")                               \
    X(ClientHTTPError, = 499, "Unexpected HTTP Client Error")       \
    X(InternalServerError, = 500, "Internal Server Error")          \
    X(NotImplemented, , "Failed To Recognize Request Method")       \
    X(BadGateway, , "Bad Gateway Or Proxy")                         \
    X(ServiceUnavailable, , "Cannot Handle Requests Temporarily")   \
    X(GatewayTimeout, , "Timed Out Receiving From Upstream Server") \
    X(HTTPVersionNotSupported, , "HTTP Version Not Supported")      \
    X(ServerHTTPError, = 599, "Unexpected HTTP Server Error")       \
    X(NetworkTransportError, = 1000, "Network Connection Failed")   \
    X(ConnectionTimeout, , "Connection Timed Out")                  \
    X(Timeout, , "Read Timeout")                                    \
    X(InvalidJson, , "Failed To Parse JSON")                        \
    X(MappingError, , "Object Mapping Failed")                      \
    X(MaxRetriesReached, , "Maximum Retry Attempts Exceeded")       \
    X(Unknown, = 2000, "An Unhandled Error Occured")


enum class ApiError {
#define AS_ENUM(name, value, desc) name value,
    API_ERROR_LIST(AS_ENUM)
#undef AS_ENUM
};

static constexpr ApiError allApiErrors[] = {
#define AS_VALUE(name, val, desc) ApiError::name,
    API_ERROR_LIST(AS_VALUE)
#undef AS_VALUE
};

inline const char* toString(ApiError error);

inline const char* getDescription(ApiError error);

static constexpr std::size_t apiErrorCount = std::size(allApiErrors);

constexpr std::array<ApiError, apiErrorCount> apiErrorList = {{
#define AS_VALUE(name, val, desc) ApiError::name,
    API_ERROR_LIST(AS_VALUE)
#undef AS_VALUE
}};

inline std::array<ApiError, apiErrorCount> getAPIErrorList();

#endif // HTTPLIBTESTING_API_ERROR_HPP
