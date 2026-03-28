#ifndef FASTCALC_HPP
#define FASTCALC_HPP

#include <string>
#include <cmath>
#include <map>

namespace fastcalc {
    inline double calc(const std::string& op, double n1, double n2 = 0) {
        // Standard Arithmetic
        if (op == "+") return n1 + n2;
        if (op == "-") return n1 - n2;
        if (op == "*") return n1 * n2;
        if (op == "/") return (n2 != 0) ? (n1 / n2) : 0;

        // Specific cases for common roots / geometry
        if (op == "sqrt")  return std::sqrt(n1);
        if (op == "cbrt")  return std::cbrt(n1);
        if (op == "hypot") return std::hypot(n1, n2);
        if (op == "pow")   return std::pow(n1, n2);
        if (op == "nthrt") return std::pow(n1, 1.0 / n2);

        // Generic Suffix Handler (e.g., "frthrt", "nnthrt", "sxthrt")
        static std::map<std::string, int> degrees = {
            {"fr", 4}, {"fv", 5}, {"sx", 6}, {"sv", 7}, {"eg", 8}, {"nn", 9}, {"tn", 10}
        };

        if (op.size() >= 4 && op.substr(op.size() - 2) == "rt") {
            std::string prefix = op.substr(0, 2);
            if (degrees.count(prefix)) {
                return std::pow(n1, 1.0 / degrees[prefix]);
            }
        }

        return 0;
    }
}

#endif
