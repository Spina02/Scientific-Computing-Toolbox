#ifndef NEWTON_COEFFICIENTS_HPP
#define NEWTON_COEFFICIENTS_HPP

#include "Interpolation.hpp"
#include "utilities_interpolation.hpp"
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include <vector>
#include <cmath> 

/** @namespace ScientificToolbox::Interpolation
 * @brief Interpolation module providing basic interpolation functions
 * 
 * This namespace contains template classes for common interpolation methods:
 * - Linear interpolation
 * - Polynomial interpolation
 * - Spline interpolation
 */

namespace ScientificToolbox::Interpolation {

    /** @class Newton class
     * @brief Newton polynomial interpolation
     * 
     * This class implements Newton polynomial interpolation using divided differences.
     * It inherits from the PolynomialInterpolation class and provides the following methods:
     * - divided_differences: Compute divided differences table
     * - newton_coefficients: Compute Newton polynomial coefficients
     * - interpolate: Interpolate a value using the Newton polynomial
     * 
     * @tparam T Data type (e.g., float, double)
     * @see PolynomialInterpolation
     */
    template <typename T>
    class Newton : public Interpolation<T> {
    public:
        /** @Constructor
         * @brief Construct a Newton polynomial interpolation object
         * 
         * This constructor initializes the Newton polynomial interpolation object with a set of data points.
         * 
         */
        explicit Newton(const std::set<point<T>>& data) : Interpolation<T>(data) {}

        /** @Destructor
         * @brief Destroy the Newton polynomial interpolation object
         * 
         * This destructor is the default destructor for the Newton polynomial interpolation object.
         * 
         */
        virtual ~Newton() = default;

        /** @method divided_differences method
         * @brief Compute divided differences table
         * 
         * This method computes the divided differences table for Newton polynomial interpolation.
         * It uses the x and y values provided in the constructor to fill the table.
         * 
         * @return Table of divided differences
         */
        std::vector<std::vector<T>> divided_differences() const{
            size_t n = this->x_.size(); // Accessing inherited x and y values
            std::vector<std::vector<T>> table(n, std::vector<T>(n, 0.0)); // Table of divided differences

            // Initialize the first column with y values
            for (size_t i = 0; i < n; ++i) {
                table[i][0] = this->y_[i];
            }

            // Compute divided differences
            for (size_t j = 1; j < n; ++j) {
                for (size_t i = 0; i < n - j; ++i) {
                    table[i][j] = (table[i + 1][j - 1] - table[i][j - 1]) / (this->x_[i + j] - this->x_[i]);
                }
            }

            return table;
        }

        /** @method newton_coefficients method
         * @brief Compute Newton polynomial coefficients
         * 
         * This method computes the Newton polynomial coefficients using the divided differences table.
         * It extracts the coefficients from the table and returns them as a vector.
         * 
         * @return Vector of Newton polynomial coefficients
         */
        std::vector<T> newton_coefficients() const{
            auto table = divided_differences();  // No need to pass x and y explicitly
            std::vector<T> coefficients;

            // Extract coefficients from the divided differences table
            for (size_t i = 0; i < table.size(); ++i) {
                coefficients.push_back(table[0][i]);
            }

            return coefficients;
        }

        /** interpolate method
         * @brief Interpolate a value using the Newton polynomial
         * 
         * This method evaluates the Newton polynomial at a given point x.
         * It uses the Newton coefficients to compute the polynomial value at x.
         * 
         * @param x Interpolation point
         * @return Interpolated value at x
         */
        T interpolate(T x) const override {
            // Obtain the Newton coefficients
            std::vector<T> coefficients = this->newton_coefficients();

            // Evaluate the Newton polynomial at x
            T result = coefficients[0]; // Start with the first coefficient

            T product = 1.0; // Product term (x - x0)(x - x1)... 
            for (size_t i = 1; i < this->x_.size(); ++i) {
                product *= (x - this->x_[i - 1]); // (x - x0), (x - x1), ...
                result += coefficients[i] * product; // Add the corresponding term
            }

            return result;
        }
    };
}

#endif
