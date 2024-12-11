#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "utilities_interpolation.hpp"

#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>

#include <set>
#include <stdexcept>
#include <vector>

/**
 * namespace ScientificToolbox::Interpolation
 * @brief Interpolation module providing basic interpolation functions
 * 
 * This namespace contains template classes for common interpolation methods:
 * - Linear interpolation
 * - Polynomial interpolation
 * - Spline interpolation
 */

/** Interpolation class
 * @brief Abstract base class for interpolation
 * 
 * This class is an abstract base class for interpolation. It contains a set of points
 * and vectors for x and y values. The class provides a pure virtual function for
 * interpolation and an overloaded operator() for calling the interpolation function.
 * 
 * @tparam T Data type
 * 
 */

namespace ScientificToolbox::Interpolation {

    // Abstract base class for interpolation
    template <typename T>
    class Interpolation {
    public:

        // Constructor
        explicit Interpolation(const std::set<point<T>>& data) : data(data) {
            if (data.empty()) {
                throw std::invalid_argument("Data set cannot be empty.");
            }

            for (const auto& point : data) {
                x_.push_back(point.get_x());
                y_.push_back(point.get_y());
            }
        }

        // Virtual destructor
        virtual ~Interpolation() = default;

        // Pure virtual interpolation function
        virtual T interpolate(T x) const = 0;

        // Overload operator()
        virtual T operator()(T x) const { return interpolate(x); }

    protected:
        std::set<point<T>> data;
        std::vector<T> x_;
        std::vector<T> y_;
    };

} // namespace ScientificToolbox

#endif
