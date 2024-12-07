#ifndef POLYNOMIALINTERPOLATION_HPP
#define POLYNOMIALINTERPOLATION_HPP

namespace ScientificToolbox {

    #include "../Utilities/Lagrange_coefficients.hpp"  // Ensure this is included to access the function
    #include "Interpolation.hpp"
    #include <gsl/gsl_poly.h>
    #include <vector>
    #include <set>  // Needed for std::set

    template <typename T>
    class PolynomialInterpolation : public Interpolation<T> {
    public:
        // Constructor
        explicit PolynomialInterpolation(const typename Interpolation<T>::set& data) : Interpolation<T>(data) {
            if (data.empty()) {
                throw std::invalid_argument("Data points cannot be empty.");
            }
        }

        // Virtual Destructor
        virtual ~PolynomialInterpolation() = default;

        // Pure virtual interpolation function
        virtual T interpolate(T x) const = 0;

        // Overload operator()
        virtual T operator()(T x) const { return interpolate(x); }

    private:
        typename Interpolation<T>::set data; // Stored interpolation data
    };

    template <typename T>
    class Lagrange : public PolynomialInterpolation<T> {
    public:
        // Constructor
        explicit Lagrange(const typename Interpolation<T>::set& data) : PolynomialInterpolation<T>(data) {
            if (data.empty()) {
                throw std::invalid_argument("Data points cannot be empty.");
            }
        }

        // Destructor
        virtual ~Lagrange() = default;

        // Interpolation function
        T interpolate(T x) const override {
            // Obtain the set of x values from data
            std::vector<T> x_values;
            for (auto it = this->getData().begin(); it != this->getData().end(); ++it) {
                x_values.push_back(it->get_x());
            }
            // Obtain the set of y values from data
            std::vector<T> y_values;
            for (auto it = this->getData().begin(); it != this->getData().end(); ++it) {
                y_values.push_back(it->get_y());
            }

            // Obtain the coefficients
            std::vector<T> coefficients = ScientificToolbox::compute_lagrange_coefficients(x_values, y_values);

            // Evaluate the polynomial at x
            T result = gsl_poly_eval(coefficients.data(), coefficients.size(), x);

            return result;
        }
    };
}

#endif
