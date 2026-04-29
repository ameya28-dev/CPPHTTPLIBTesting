#include "header/date/dob.hpp"

#include "date/date.h"
#include <chrono>

#if !NDEBUG
#include <iostream>
#include <type_traits>
#endif

#include <stdexcept>

#include "lib/logger/manager/manager.hpp"

date::year_month_day Person::ConstructValidDate(const int year, const int month, const int day) {
    auto date = date::year{year} / static_cast<date::month>(month) / day;
    if (!date.ok()) {
        throw std::invalid_argument("Invalid arguments resulting in non existent date creation!!!");
    }
    return date;
}

Person::Person(const char* name, const int year, const int month, const int day)
    : name(name), dateOfBirth(ConstructValidDate(year, month, day)) {}

int Person::getAge() const {
    const auto startSys = date::sys_days{dateOfBirth};
    const auto endSys   = date::sys_days{date::floor<date::days>(std::chrono::system_clock::now())};
    const auto duration = endSys - startSys;
    return duration.count() / NoOfDaysInAYear;
}

int Person::getAgeInDays() const {
    const auto startSys = date::sys_days{dateOfBirth};
    const auto endSys   = date::sys_days{date::floor<date::days>(std::chrono::system_clock::now())};
    return (endSys - startSys).count();
}

void checkSizeAndAlignmentOfDate() {
#if !NDEBUG
    std::cout << "Size of date::year_month_day = " << sizeof(date::year_month_day) << '\n';
    std::cout << "Alignment of date::year_month_day = " << std::alignment_of<date::year_month_day>() << '\n';
#endif
}

void getCurrentAge() {
    try {
        Person montgomery{"Montgomery Clift", 1920, 10, 17};
        PRINT_INFO("{} is {} days old", montgomery.name, montgomery.getAgeInDays());
        Person james{"James Dean", 1931, 2, 29};
        PRINT_INFO("{} is {} years old", james.name, james.getAge());
    } catch (const std::invalid_argument& ex) {
        LOG_FATAL("Person could not be created!!: {}", ex.what());
    }
}
