#ifndef CUBIC_SPLINE_INTERPOLATION_HPP
#define CUBIC_SPLINE_INTERPOLATION_HPP

#include "Interpolation.hpp"
#include "utilities_interpolation.hpp"
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>

#include <vector>
#include <iostream>



namespace ScientificToolbox::Interpolation {

    template <typename T>
    class CubicSplineInterpolation : public Interpolation<T> {
    public:
        // Constructor
        explicit CubicSplineInterpolation(const typename Interpolation<T>::point_set& data) : Interpolation<T>(data) {
            if (data.empty()) {
                throw std::invalid_argument("Data points cannot be empty.");
            }

            // Convert point_set to vectors
            std::pair<std::vector<T>, std::vector<T>> vectors = this->toVectors();
            x = vectors.first;  
            y = vectors.second;  

            if(DEBUG){
                // printing the x and y values
                std::cout << "x values: ";
                for (size_t i = 0; i < x.size(); ++i) {
                    std::cout << x[i] << " ";
                }
                std::cout << std::endl;
                std::cout << "y values: ";
                for (size_t i = 0; i < y.size(); ++i) {
                    std::cout << y[i] << " ";
                }
                std::cout << std::endl;
            }

            // Compute the step size
            T step_size = x[1] - x[0];

            if (DEBUG){
                std::cout << "Step size: " << step_size << std::endl;
            }

            // Check uniform spacing
            double epsilon = 1e-6;
            for (size_t i=1; i < x.size(); ++i){
                if (std::abs(x[i] - x[i-1] - step_size) > epsilon){
                    throw std::invalid_argument("Data points must be uniformly spaced for cubic spline interpolation.");
                }
            }

            spline = boost::math::interpolators::cardinal_cubic_b_spline<T>(y.data(), y.size(), x[0], step_size);
        }

        // Destructor
        ~CubicSplineInterpolation() = default;

        // Interpolation function
        T interpolate(T x_query) const override {
            return spline(x_query);
        }

    protected:
        std::vector<T> x;
        std::vector<T> y;
        boost::math::interpolators::cardinal_cubic_b_spline<T> spline;
        const bool DEBUG = true;
    };
}

#endif
