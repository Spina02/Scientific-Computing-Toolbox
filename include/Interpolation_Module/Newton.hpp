#ifndef NEWTON_COEFFICIENTS_HPP
#define NEWTON_COEFFICIENTS_HPP

#include "Interpolation.hpp"
#include "PolynomialInterpolation.hpp"
#include <vector>
#include <cmath> 

namespace ScientificToolbox {

    template <typename T>
    class Newton : public PolynomialInterpolation<T> {
    public:
        // Destructor
        virtual ~Newton() = default;

        // Function to compute divided differences
        std::vector<std::vector<T>> divided_differences() {
            size_t n = this->x.size(); // Accessing inherited x and y values
            std::vector<std::vector<T>> table(n, std::vector<T>(n, 0.0)); // Table of divided differences

            // Initialize the first column with y values
            for (size_t i = 0; i < n; ++i) {
                table[i][0] = this->y[i];
            }

            // Compute divided differences
            for (size_t j = 1; j < n; ++j) {
                for (size_t i = 0; i < n - j; ++i) {
                    table[i][j] = (table[i + 1][j - 1] - table[i][j - 1]) / (this->x[i + j] - this->x[i]);
                }
            }

            return table;
        }

        // Function to compute Newton polynomial coefficients
        std::vector<T> newton_coefficients() {
            auto table = divided_differences();  // No need to pass x and y explicitly
            std::vector<T> coefficients;

            // Extract coefficients from the divided differences table
            for (size_t i = 0; i < table.size(); ++i) {
                coefficients.push_back(table[0][i]);
            }

            return coefficients;
        }

        // Interpolation function
        T interpolate(T x) const override {
            // Obtain the Newton coefficients
            std::vector<T> coefficients = this->newton_coefficients();

            // Evaluate the Newton polynomial at x
            T result = coefficients[0]; // Start with the first coefficient

            T product = 1.0; // Product term (x - x0)(x - x1)... 
            for (size_t i = 1; i < this->x.size(); ++i) {
                product *= (x - this->x[i - 1]); // (x - x0), (x - x1), ...
                result += coefficients[i] * product; // Add the corresponding term
            }

            return result;
        }
    };
}

#endif
