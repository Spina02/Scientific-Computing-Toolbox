#ifndef LAGRANGE_COEFFICIENTS_HPP
#define LAGRANGE_COEFFICIENTS_HPP

#include "Interpolation.hpp"
#include "PolynomialInterpolation.hpp"
#include <gsl/gsl_poly.h>
#include <vector>

namespace ScientificToolbox {

    template <typename T>
    class Lagrange : public ScientificToolbox::PolynomialInterpolation<T> {
    public:

        // Destructor
        ~Lagrange() = default;

        // Computing Lagrange coefficients
        std::vector<T> compute_lagrange_coefficients() {
            size_t n = this->x.size(); // Use inherited x and y values
            std::vector<T> polynomial(n, 0.0); // Resulting polynomial coefficients

            for (size_t i = 0; i < n; ++i) {
                std::vector<T> basis(n, 0.0); // Coefficients of L_i(x)
                basis[0] = 1.0; // Start with a constant term for L_i(x)

                // Compute L_i(x)
                for (size_t j = 0; j < n; ++j) {
                    if (i != j) {
                        // Update basis polynomial for (x - x_j) / (x_i - x_j)
                        for (size_t k = n - 1; k > 0; --k) {
                            basis[k] = basis[k] * (-this->x[j]) + basis[k - 1];
                        }
                        basis[0] *= -this->x[j];

                        // Divide by (x_i - x_j)
                        for (size_t k = 0; k < n; ++k) {
                            basis[k] /= (this->x[i] - this->x[j]);
                        }
                    }
                }

                // Add y[i] * L_i(x) to the polynomial
                for (size_t k = 0; k < n; ++k) {
                    polynomial[k] += this->y[i] * basis[k];
                }
            }

            return polynomial;
        }

        // Interpolation function
        T interpolate(T x) const override {
            // Obtain the coefficients
            std::vector<T> coefficients = this->compute_lagrange_coefficients();

            // Evaluate the polynomial at x
            T result = gsl_poly_eval(coefficients.data(), coefficients.size(), x);

            return result;
        }
    };

}

#endif
