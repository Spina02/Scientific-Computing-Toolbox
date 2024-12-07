#ifndef LAGRANGE_COEFFICIENTS_HPP
#define LAGRANGE_COEFFICIENTS_HPP

#include <vector>

namespace ScientificToolbox{
    std::vector<double> compute_lagrange_coefficients(const std::vector<double>& x, const std::vector<double>& y) {
        size_t n = x.size();
        std::vector<double> polynomial(n, 0.0); // Resulting polynomial coefficients

        for (size_t i = 0; i < n; ++i) {
            std::vector<double> basis(n, 0.0); // Coefficients of L_i(x)
            basis[0] = 1.0; // Start with a constant term for L_i(x)

            // Compute L_i(x)
            for (size_t j = 0; j < n; ++j) {
                if (i != j) {
                    // Update basis polynomial for (x - x_j) / (x_i - x_j)
                    for (size_t k = n - 1; k > 0; --k) {
                        basis[k] = basis[k] * (-x[j]) + basis[k - 1];
                    }
                    basis[0] *= -x[j];

                    // Divide by (x_i - x_j)
                    for (size_t k = 0; k < n; ++k) {
                        basis[k] /= (x[i] - x[j]);
                    }
                }
            }

            // Add y[i] * L_i(x) to the polynomial
            for (size_t k = 0; k < n; ++k) {
                polynomial[k] += y[i] * basis[k];
            }
        }

        return polynomial;
    }
}

#endif