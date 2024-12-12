#ifndef LAGRANGE_COEFFICIENTS_HPP
#define LAGRANGE_COEFFICIENTS_HPP

#include "Interpolation.hpp"
#include "utilities_interpolation.hpp"
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include <vector>

/** @namespace ScientificToolbox::Interpolation
 * @brief Interpolation module providing basic interpolation functions
 * 
 * This namespace contains template classes for common interpolation methods:
 * - Linear interpolation
 * - Polynomial interpolation
 * - Spline interpolation
 */

namespace ScientificToolbox::Interpolation {

    /** @class Lagrange class
     * @brief Lagrange interpolation class
     * 
     * This class provides a Lagrange interpolation method.
     * The Lagrange interpolation method is a polynomial interpolation method that uses a set of points to interpolate a value.
     * 
     * @tparam T Data type
     * 
     */
    template <typename T>
    class Lagrange : public Interpolation<T> {
    public:
        /** @Constructor
         * @brief Constructor for the Lagrange interpolation class
         * 
         * This constructor initializes the Lagrange interpolation class with a set of points.
         * 
         * @param data Set of points
         * 
         */
        explicit Lagrange(const std::set<point<T>>& data) : Interpolation<T>(data) {
            // Create the spline
            acc = gsl_interp_accel_alloc();
            spline = gsl_spline_alloc(gsl_interp_polynomial, this->x_.size());
        }

        /** @Destructor
         * @brief Destructor for the Lagrange interpolation class
         * 
         * This destructor frees the memory allocated for the spline and the interpolation accelerator.
         * 
         */
        ~Lagrange() {
            gsl_spline_free(spline);
            gsl_interp_accel_free(acc);
        }

        /** @method interpolate method
         * @brief Interpolate a value using the Lagrange polynomial
         * 
         * This method evaluates the Lagrange polynomial at a given point x.
         * The Lagrange polynomial is computed using the coefficients obtained from the given points.
         * 
         * @param x Value to interpolate
         * @return T Interpolated value at x
         * 
         */
        T interpolate(T x_query) const override {
            gsl_spline_init(spline, this->x_.data(), this->y_.data(), this->x_.size());
            return gsl_spline_eval(spline, x_query, acc);
        }

    protected:
        gsl_interp_accel *acc;
        gsl_spline *spline;
    };

}

#endif
