#ifndef SMALL_CLASSES_HPP
#define SMALL_CLASSES_HPP

#include <vector>
#include <utility> // For std::pair
#include <stdexcept> // For exceptions
#include <algorithm> // For std::lower_bound, std::sort
#include <limits> // For std::numeric_limits
#include <set> // For std::set

/**
 * namespace ScientificToolbox::Interpolation
 * @brief Interpolation module providing basic interpolation functions
 * 
 * This namespace contains template classes for common interpolation methods:
 * - Linear interpolation
 * - Polynomial interpolation
 * - Spline interpolation
 */

/** point class
 * @brief Class for storing data points
 * @tparam T Type of x and y coordinates
 * 
 * This class stores a pair of x and y coordinates as a data point.
 * It provides methods to access the x and y coordinates.
 * 
 */

/** abline class
 * @brief Class for linear interpolation
 * @tparam T Type of x and y coordinates
 * 
 * This class represents a line connecting two points.
 * It provides methods to evaluate the line at a given x coordinate.
 * 
 */

/** interval class
 * @brief Class for storing interval bounds
 * @tparam T Type of interval bounds
 * 
 * This class stores a pair of lower and upper bounds as an interval.
 * It provides methods to access the lower and upper bounds, and to check if a value is within the interval.
 * 
 */

/** minmax_search function
 * @brief Function to find the minimum and maximum x values in a set of points
 * @tparam T Type of x and y coordinates
 * 
 * This function takes a set of points and returns the minimum and maximum x values.
 * 
 */

namespace ScientificToolbox::Interpolation {
    // point class
    template <typename T>
    class point {
    public:
        point(T x, T y) : x(x), y(y) {}
        ~point() = default;

        T get_x() const { return x; }
        T get_y() const { return y; }

        // Custom comparison operator to allow using point in a set
        bool operator<(const point<T>& other) const {
            return x < other.x || (x == other.x && y < other.y);
        }

    private:
        T x;
        T y;
    };

    // interval class
    template <typename T>
    class interval {
    public:
        interval(T lower_bound, T upper_bound) 
            : lower_bound(lower_bound), upper_bound(upper_bound) {
            if (lower_bound > upper_bound) {
                throw std::invalid_argument("Invalid interval: check input data.");
            }
        }

        ~interval() = default;

        T get_lower_bound() const { return lower_bound; }
        T get_upper_bound() const { return upper_bound; }
        std::pair<T, T> get_interval() const { return {lower_bound, upper_bound}; }
        T get_difference() const { return upper_bound - lower_bound; }

        bool contains(T x) const {
            return x >= lower_bound && x <= upper_bound;
        }

    private:
        T lower_bound;
        T upper_bound;
    };

    // Minmax search
    template <typename T>
    interval<T> minmax_search(const std::set<point<T>>& points) {
        if (points.empty()) {
            throw std::invalid_argument("Cannot find minmax: empty set of points.");
        }

        T min = std::numeric_limits<T>::max();
        T max = std::numeric_limits<T>::lowest();

        for (const auto& p : points) {
            if (p.get_x() < min) {
                min = p.get_x();
            }
            if (p.get_x() > max) {
                max = p.get_x();
            }
        }

        return interval<T>(min, max);
    }
}

#endif
