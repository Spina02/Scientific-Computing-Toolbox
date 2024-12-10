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
 * @tparam T Data type for interpolation
 * 
 * This class provides a common interface for interpolation methods.
 * It stores a set of data points and provides a virtual function for interpolation.
 * The class also provides a method to convert the data to a pair of std::vector<T>s.
 * 
 * The class is templated on the data type T and requires a point class with get_x() and get_y() methods.
 * The class also provides an overloaded operator() for interpolation.
 * 
 */

/** std::set<point<T>>
 * @brief Alias for a set of points
 * 
 * This alias is used to define the data structure for interpolation.
 * It is a set of points with a custom comparison operator.
 * 
 */

/** Constructor
 * @brief Constructs an interpolation object with a set of data points
 * @param data Set of data points
 * @throws std::invalid_argument if data is empty
 */

/** Destructor
 * @brief Virtual destructor for interpolation class
 */

/** tostd::vector<T>s
 * @brief Converts data points to a pair of std::vector<T>s
 * @return Pair of std::vector<T>s (x, y) representing the data points
 */

/** interpolate
 * @brief Pure virtual function for interpolation
 * @param x Interpolation point
 * @return Interpolated value at point x
 */

/** operator()
 * @brief Overloaded operator for interpolation
 * @param x Interpolation point
 * @return Interpolated value at point x
 */

/** getData
 * @brief Getter for data points
 * @return Reference to the data points
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
