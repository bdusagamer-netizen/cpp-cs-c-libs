#ifndef SUPERMATH_HPP
#define SUPERMATH_HPP

#include <iostream>
#include <random>
#include <cmath>
#include <initializer_list>
#include <vector>
#include <type_traits>

namespace supermath {

    static std::random_device rd;
    static std::mt19937 gen(rd());

    template <typename... Args>
    auto randomlist(Args... args) {
        // Automatically finds a common type for all arguments
        using CommonType = std::common_type_t<Args...>;
        std::vector<CommonType> values = { static_cast<CommonType>(args)... };
        
        std::uniform_int_distribution<size_t> dis(0, values.size() - 1);
        return values[dis(gen)];
    }

    int random(int min, int max) {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(gen);
    }

    double getDouble(double min, double max) {
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(gen);
    }

    int add(int a, int b) { return a + b; }
    int sub(int a, int b) { return a - b; }
    int mul(int a, int b) { return a * b; }
    double div(double a, double b) { return (b != 0) ? a / b : 0; }
    int mod(int a, int b) { return a % b; }

    double sqrt(double n) { return std::sqrt(n); }
    double cbrt(double n) { return std::cbrt(n); }
    double nthrt(double n, double root) { return std::pow(n, 1.0 / root); }
    int pow(int base, int exp) { return static_cast<int>(std::pow(base, exp)); }
    double hypot(double a, double b) { return std::sqrt(std::pow(a, 2) + std::pow(b, 2)); }
}

#endif
