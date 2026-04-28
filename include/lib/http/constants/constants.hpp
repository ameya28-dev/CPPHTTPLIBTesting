#ifndef HTTPLIBTESTING_LIB_HTTP_CONSTANTS_HPP
#define HTTPLIBTESTING_LIB_HTTP_CONSTANTS_HPP

namespace ServiceConstants {
    struct Defaults {
        static constexpr auto Verbose             = false;
        static constexpr auto RetryCount          = 3;
        static constexpr auto JSONApplicationType = "application/json";
    };
}; // namespace ServiceConstants

#endif // HTTPLIBTESTING_LIB_HTTP_CONSTANTS_HPP
