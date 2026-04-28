#include "lib/random/random.hpp"

UniformInt::UniformInt() : _gen(std::random_device{}()), _distribution(0, 9) {}

UniformInt::UniformInt(const int min, const int max) : _gen(std::random_device{}()), _distribution(min, max) {}

auto UniformInt::next() -> int {
    return _distribution(_gen);
}

auto UniformInt::getVector(const std::size_t size) -> std::vector<int> {
    std::vector<int> res(size);
    for (size_t i = 0; i < size; i++) {
        res[i] = _distribution(_gen);
    }
    return res;
}

UniformReal::UniformReal() : _gen(std::random_device{}()) {}

UniformReal::UniformReal(const double min, const double max) : _gen(std::random_device{}()), _distribution(min, max) {}

UniformReal::UniformReal(const double min, const double max, const bool inclusive) : _gen(std::random_device{}()) {
    if (inclusive) {
        _distribution = std::uniform_real_distribution<>(min, std::nextafter(max, std::numeric_limits<double>::max()));
    } else {
        _distribution = std::uniform_real_distribution<>(min, max);
    }
}

auto UniformReal::next() -> double {
    return _distribution(_gen);
}

auto UniformReal::getVector(const std::size_t size) -> std::vector<double> {
    std::vector<double> res(size);
    for (int i = 0; i < size; i++) {
        res[i] = _distribution(_gen);
    }
    return res;
}

Normal::Normal() : _gen(std::random_device{}()) {}

Normal::Normal(const double mean, const double stdDev) : _gen(std::random_device{}()), _distribution(mean, stdDev) {}

auto Normal::next() -> double {
    return _distribution(_gen);
}

auto Normal::getVector(const std::size_t size) -> std::vector<double> {
    std::vector<double> res(size);
    for (int i = 0; i < size; i++) {
        res[i] = _distribution(_gen);
    }
    return res;
}
