#ifndef LINEARINTERPOLATION_HPP
#define LINEARINTERPOLATION_HPP

#include "Interpolation.hpp"
#include "utilities_interpolation.hpp"

#include <set>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace ScientificToolbox {

    // Derived class for linear interpolation
    template <typename T>
    class LinearInterpolation : public Interpolation<T> {
    public:
        // Alias for the set of data points
        using point_set = typename Interpolation<T>::point_set;  // Simplified `using` declaration

        // Constructor - computes the interpolated functions
        LinearInterpolation(const point_set& data) : Interpolation<T>(data) {
            auto current_it = data.begin(); // Iterator to the first element
            auto previous_it = current_it;  // Iterator to the previous element
            ++current_it;                   // Iterator to the next element

            // Loop through the data points
            while (current_it != data.end()) {
                interval<T> intv(previous_it->get_x(), current_it->get_x()); // Create an interval
                abline<T> ab(*previous_it, *current_it);                      // Create a line
                interpolated_functions.push_back(std::make_pair(intv, ab));   // Store the interpolated function

                previous_it = current_it; // Move the previous iterator
                ++current_it;             // Move the current iterator
            }
        }

        // Implement the interpolate method
        T interpolate(T x) const override {
            // Find the interval containing x
            auto it = std::find_if(interpolated_functions.begin(), interpolated_functions.end(),
                [x](const std::pair<interval<T>, abline<T>>& p) { return p.first.contains(x); });

            // Check if the value is outside the interpolation range
            if (it == interpolated_functions.end()) {
                throw std::out_of_range("Value " + std::to_string(x) + " is outside the interpolation range.");
            }

            // Evaluate the interpolated function
            return it->second.evaluate(x);
        }

        // Overload operator()
        T operator()(T x) const override {
            return interpolate(x); // Call the interpolate method and return the result
        }

    private:
        std::vector<std::pair<interval<T>, abline<T>>> interpolated_functions; // Interpolated functions at each interval
    };

}

#endif
