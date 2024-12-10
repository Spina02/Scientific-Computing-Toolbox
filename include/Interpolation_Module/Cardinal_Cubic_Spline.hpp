#ifndef CARDINAL_CUBIC_SPLINE_HPP
#define CARDINAL_CUBIC_SPLINE_HPP

#include "Interpolation.hpp"
#include "utilities_interpolation.hpp"
#include "Cubic_Spline_Interpolation.hpp"
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>
#include <vector>

namespace ScientificToolbox::Interpolation {

    template <typename T>
    class CardinalCubicSpline : public CubicSplineInterpolation<T> {
    public:
        // Constructor
        explicit CardinalCubicSpline(const typename Interpolation<T>::point_set& data) : CubicSplineInterpolation<T>(data) {
            
            auto step = this->x[1] - this->x[0];
            if (this->DEBUG){
                // printing step
                std::cout << "Step: " << step << std::endl;
            }

            // Checking in the x values are uniformly spaced
            for (size_t i = 1; i < this->x.size(); ++i) {
                if (std::abs(this->x[i] - this->x[i - 1] - step) > 1e-6) {
                    throw std::invalid_argument("X values must be uniformly spaced to compute this interpolation.");
                }
            }

            // Create the spline
            this->spline = boost::math::interpolators::cardinal_cubic_b_spline<T>(this->y.data(), this->y.size(), this->x[0], step);
        }

        // Destructor
        ~CardinalCubicSpline() = default;

        // Interpolation function
        T interpolate(T x_query) const override {
            return this->spline(x_query);
        }

    protected:
        boost::math::interpolators::cardinal_cubic_b_spline<T> spline;
    };
}

#endif