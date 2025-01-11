#ifndef SMALL_CLASSES_HPP
#define SMALL_CLASSES_HPP

#include "../Utilities.hpp"
#include <vector>
#include <utility> // For std::pair
#include <stdexcept> // For exceptions
#include <algorithm> // For std::lower_bound, std::sort
#include <limits> // For std::numeric_limits
#include <set> // For std::set

/** @namespace ScientificToolbox::Interpolation
 * @brief Interpolation module providing basic interpolation functions
 * 
 * This namespace contains template classes for common interpolation methods:
 * - Linear interpolation
 * - Polynomial interpolation
 * - Spline interpolation
 */

namespace ScientificToolbox::Interpolation {
    /** @class point class
     * @brief Class for storing data points
     * @tparam T Type of x and y coordinates
     * 
     * This class stores a pair of x and y coordinates as a data point.
     * It provides methods to access the x and y coordinates.
     * 
     */
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

    /** @class interval class
     * @brief Class for storing interval bounds
     * @tparam T Type of interval bounds
     * 
     * This class stores a pair of lower and upper bounds as an interval.
     * It provides methods to access the lower and upper bounds, and to check if a value is within the interval.
     * 
     */
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

    /** @class minmax_search function
     * @brief Function to find the minimum and maximum x values in a set of points
     * @tparam T Type of x and y coordinates
     * 
     * This function takes a set of points and returns the minimum and maximum x values.
     * 
     */
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

    /**  read_points_from_csv
     * @brief Reads a CSV file and returns a set of points
     * @tparam T The data type of the points
     * @param filename The path to the CSV file
     * @return Set of points read from the CSV file
     * 
     * This method reads a CSV file containing x,y pairs of data and returns a set of points
     * of type T. The CSV file is expected to have a format where each line contains two values
     * separated by a comma, representing the x and y coordinates of a point. The points are
     * stored in a set to ensure uniqueness and sorted order. The sorted order is important here since 
     * the points are used for interpolation, and the interpolation methods require the points to be sorted.
     * Also, the set ensures that there are no duplicate points in the data.
     */
    template <typename T>
    std::set<point<T>> read_points_from_csv(const std::string& filename) {
        std::set<point<T>> points;
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Could not open the file!");
        }

        std::string line;

        // Skip the first line (header)
        std::getline(file, line);
        if (DEBUG)
            // Debugging header
            std::cout << "Skipped header: " << line << std::endl;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            T x, y;
            char comma;

            if (DEBUG)
                // Debugging: Print the line being read
                std::cout << "Reading line: " << line << std::endl;

            if (ss >> x >> comma >> y) {
                if (DEBUG)
                    // Debugging: Print parsed values
                    std::cout << "Parsed values: x = " << x << ", y = " << y << std::endl;
                points.insert(point<T>(x, y));
            } else {
                std::cerr << "Failed to parse line: " << line << std::endl;
                throw std::runtime_error("Failed to parse numeric values from the CSV file. Check the file content, it should not contain characters or strings but only two column with x and y values respectively.");
            }

            // printing the points
            if (DEBUG) {
                std::cout << "Points: ";
                for (const auto& p : points) {
                    std::cout << "(" << p.get_x() << ", " << p.get_y() << ") ";
                }
                std::cout << std::endl;
            }
        }

        file.close();
        return points;
    }
}

#endif
