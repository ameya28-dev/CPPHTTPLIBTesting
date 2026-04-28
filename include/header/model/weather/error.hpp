#include <fmt/format.h>
#include <string>

#include <nlohmann/json.hpp>

namespace weather {
    struct WeatherError {
        std::string cod;
        std::string message;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(WeatherError, cod, message)
    };
} // namespace weather

template <>
struct fmt::formatter<weather::WeatherError> : formatter<std::string> {
    constexpr auto parse(format_parse_context& context) {
        return formatter<std::string>::parse(context);
    }

    static auto format(const weather::WeatherError& err, const format_context& context) {
        return format_to(context.out(), "WeatherError(cod: {}, message: {})", err.cod, err.message);
    }
};
