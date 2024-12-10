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
        }

        // Destructor
        ~CubicSplineInterpolation() = default;

        // Interpolation function
        virtual T interpolate(T x) const = 0;

        // Overload operator()
        virtual T operator()(T x) const { return interpolate(x); }

    protected:
        std::vector<T> x;
        std::vector<T> y;
        const bool DEBUG = true;
    };
}

#endif
