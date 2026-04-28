#include "lib/http/error/api_error.hpp"

const char* toString(const ApiError error) {
    switch (error) {
#define AS_CASE_NAME(name, value, desc) \
    case ApiError::name:                \
        return #name API_ERROR_LIST(AS_CASE_NAME)
#undef AS_CASE_NAME
    default:
        return "Unknown";
    }
}

const char* getDescription(const ApiError error) {
    switch (error) {
#define AS_CASE_DESC(name, value, desc) \
    case ApiError::name:                \
        return desc;                    \
        API_ERROR_LIST(AS_CASE_DESC)
#undef AS_CASE_DESC
    default:
        return "Unknown";
    }
}

std::array<ApiError, apiErrorCount> getAPIErrorList() {
    return apiErrorList;
}
