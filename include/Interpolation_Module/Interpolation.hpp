#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "utilities_interpolation.hpp"
#include <set>
#include <vector>  // Include the header for vector
#include <stdexcept>

namespace ScientificToolbox {

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
