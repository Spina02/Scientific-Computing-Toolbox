#ifndef LINEARINTERPOLATION_HPP
#define LINEARINTERPOLATION_HPP

#include "Interpolation.hpp"
#include "utilities_interpolation.hpp"

#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>

#include <set>
#include <vector>
#include <algorithm>
#include <stdexcept>

/** @namespace ScientificToolbox::Interpolation
 * @brief Interpolation module providing basic interpolation functions
 * 
 * This namespace contains template classes for common interpolation methods:
 * - Linear interpolation
 * - Polynomial interpolation
 * - Spline interpolation
 */

namespace ScientificToolbox::Interpolation {

    /** @class LinearInterpolation
     * @brief Linear interpolation class
     * 
     * This class implements linear interpolation using the `Interpolation` base class.
     * It computes the interpolated functions at each interval and provides the `interpolate` method.
     * 
     * @tparam T Data type of the x and y values
     * 
     * @see Interpolation
     * 
     */
    template <typename T>
    class LinearInterpolation : public Interpolation<T> {
    public:
        /** @Constructor
         * @brief Constructor for linear interpolation
         * 
         * This constructor initializes the linear interpolation object with a set of points.
         * It also initializes the GSL interpolation accelerator and spline.
         * 
         * @param data Set of points to interpolate
         * 
         */
        explicit LinearInterpolation(const std::set<point<T>>& data) : Interpolation<T>(data) {
            // Create the spline
            acc = gsl_interp_accel_alloc();
            spline = gsl_spline_alloc(gsl_interp_linear, this->x_.size());
        }

        /** @Destructor
         * @brief Destructor for linear interpolation
         * 
         * This destructor frees the GSL interpolation accelerator and spline.
         * 
         */
        ~LinearInterpolation() {
            gsl_spline_free(spline);
            gsl_interp_accel_free(acc);
        }

        /** interpolate
         * @brief Interpolate method for linear interpolation
         * 
         * This method evaluates the interpolated function at a given x value.
         * It finds the interval containing the x value and evaluates the linear function at that interval.
         * 
         * @param x Value at which to interpolate
         * 
         * @return Interpolated value at x
         * 
         */
        T interpolate(T x_query) const override {
            gsl_spline_init(spline, this->x_.data(), this->y_.data(), this->x_.size());
            return gsl_spline_eval(spline, x_query, acc);
        }
    
    protected:
        gsl_interp_accel* acc;  // GSL interpolation accelerator
        gsl_spline* spline;     // GSL interpolation spline
    };

}

#endif
