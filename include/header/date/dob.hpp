#ifndef HTTPLIBTESTING_HEADER_DATE_DOB_HPP
#define HTTPLIBTESTING_HEADER_DATE_DOB_HPP

#include <date/date.h>

namespace date {
    inline auto format_as(const date::year_month_day ymd) {
        return date::format("%F", ymd);
    }
} // namespace date

static constexpr int NoOfDaysInAYear = 365;

struct Person {
    std::string name;
    date::year_month_day dateOfBirth;

    explicit Person(const char*, int, int, int);

    static date::year_month_day ConstructValidDate(int, int, int);

    [[nodiscard]] int getAge() const;

    [[nodiscard]] int getAgeInDays() const ;
};

void checkSizeAndAlignmentOfDate();

void getCurrentAge();

#endif
