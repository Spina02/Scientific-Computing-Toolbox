#ifndef PCHIP_HPP
#define PCHIP_HPP

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
    class PchipInterpolation : public CubicSplineInterpolation<T> {
    public:
        // Constructor
        explicit PchipInterpolation(const typename Interpolation<T>::point_set& data) : CubicSplineInterpolation<T>(data) {
            if (data.empty()) {
                throw std::invalid_argument("Data points cannot be empty.");
            }

            x = this->toVectors().first;
            y = this->toVectors().second;

            // Create the spline
            acc = gsl_interp_accel_alloc();
            spline = gsl_spline_alloc(gsl_interp_cspline, x.size());
        }

        // Destructor
        ~PchipInterpolation() {
            gsl_spline_free(spline);
            gsl_interp_accel_free(acc);
        }

        // Interpolation function
        T interpolate(T x_query) const override {
            gsl_spline_init(spline, x.data(), y.data(), x.size());
            return gsl_spline_eval(spline, x_query, acc);
        }

    protected:
        std::vector<T> x;
        std::vector<T> y;
        gsl_interp_accel *acc;
        gsl_spline *spline;
        const bool DEBUG = true;
    };
}

#endif