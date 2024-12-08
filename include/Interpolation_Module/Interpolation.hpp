#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "utilities_interpolation.hpp"
#include <set>
#include <vector>  // Include the header for vector
#include <stdexcept>

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
 * The class also provides a method to convert the data to a pair of vectors.
 * 
 * The class is templated on the data type T and requires a point class with get_x() and get_y() methods.
 * The class also provides an overloaded operator() for interpolation.
 * 
 */

/** point_set
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

/** toVectors
 * @brief Converts data points to a pair of vectors
 * @return Pair of vectors (x, y) representing the data points
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
        using point_set = std::set<point<T>>;  // Renamed alias to avoid conflict

        // Constructor
        explicit Interpolation(const point_set& data) : data(data) {
            if (data.empty()) {
                throw std::invalid_argument("Data points cannot be empty.");
            }
        }

        // Virtual destructor
        virtual ~Interpolation() = default;

        // Method to change data structure to a pair of vectors
        std::pair<std::vector<T>, std::vector<T>> toVectors() const {
            std::vector<T> x, y;
            for (const auto& p : data) {
                x.push_back(p.get_x());  // Corrected access to x coordinate
                y.push_back(p.get_y());  // Corrected access to y coordinate
            }
            return {x, y};
        }

        // Pure virtual interpolation function
        virtual T interpolate(T x) const = 0;

        // Overload operator()
        virtual T operator()(T x) const { return interpolate(x); }

    protected:
        // Getter for data
        const point_set& getData() const { return data; }

    private:
        point_set data;  // Stored interpolation data
    };

} // namespace ScientificToolbox

#endif
