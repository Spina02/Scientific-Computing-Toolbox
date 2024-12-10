#ifndef CUBIB_SPLINE_INTERPOLATION_HPP
#define CUBIB_SPLINE_INTERPOLATION_HPP

#include "Interpolation.hpp"
#include "Cubic_Spline_Interpolation.hpp"
#include "utilities_interpolation.hpp"
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace ScientificToolbox::Interpolation {

    template <typename T>
    class SplineInterpolation : public Interpolation<T> {
    public:
        // Constructor
        explicit SplineInterpolation(const std::set<point<T>>& data) : Interpolation<T>(data) {
            // Create the spline
            acc = gsl_interp_accel_alloc();
            spline = gsl_spline_alloc(gsl_interp_cspline, this->x_.size());
        }

        // Destructor
        ~SplineInterpolation() {
            gsl_spline_free(spline);
            gsl_interp_accel_free(acc);
        }

        // Interpolation function
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