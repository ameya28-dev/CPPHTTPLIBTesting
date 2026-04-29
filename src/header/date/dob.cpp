#include "header/date/dob.hpp"

#if !NDEBUG
#include <iostream>
#include <type_traits>
#endif

#include <stdexcept>

#include "lib/logger/manager/manager.hpp"

Person::Person(const char* name, const int year, const int month, const int day) : name(name) {
    dateOfBirth = date::year{year} / static_cast<date::month>(month) / day;
    if (!dateOfBirth.ok()) {
        throw std::invalid_argument("Invalid arguments resulting in non existent date creation!!!");
    }
}

int Person::getAge() const {
    const auto startSys = date::sys_days{dateOfBirth};
    const auto endSys   = date::sys_days{date::floor<date::days>(std::chrono::system_clock::now())};
    const auto duration = endSys - startSys;
    return duration.count() / NoOfDaysInAYear;
}

void checkSizeAndAlignmentOfDate() {
#if !NDEBUG
    std::cout << "Size of date::year_month_day = " << sizeof(date::year_month_day) << '\n';
    std::cout << "Alignment of date::year_month_day = " << std::alignment_of<date::year_month_day>() << '\n';
#endif
}

void dateCurrentAge() {
    Person person{"James Dean", 2000, 2, 27};
    PRINT_INFO("{} is {} years old", person.name, person.getAge());
}
