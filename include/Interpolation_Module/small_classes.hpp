#ifndef SMALL_CLASSES_HPP
#define SMALL_CLASSES_HPP

#include <vector>
#include <utility> // For std::pair
#include <stdexcept> // For exceptions
#include <algorithm> // For std::lower_bound, std::sort
#include <limits> // For std::numeric_limits

namespace ScientificToolbox {
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

    // abline class
    template <typename T>
    class abline {
    public:
        abline(const point<T> &p1, const point<T> &p2) {
            T dx = p2.get_x() - p1.get_x();
            if (dx == 0) {
                throw std::invalid_argument("Cannot create abline: duplicate x values.");
            }
            slope = (p2.get_y() - p1.get_y()) / dx;
            intercept = p1.get_y() - slope * p1.get_x();
        }

        ~abline() = default;

        std::pair<T, T> get_abline() const {
            return std::make_pair(slope, intercept);
        }

        T evaluate(T x) const {
            return slope * x + intercept;
        }

    private:
        T slope;
        T intercept;
    };

    // interval class
    template <typename T>
    class interval {
    public:
        interval(T lower_bound, T upper_bound) 
            : lower_bound(lower_bound), upper_bound(upper_bound) {
            if (lower_bound >= upper_bound) {
                throw std::invalid_argument("Invalid interval: lower bound must be less than upper bound.");
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
}

#endif
