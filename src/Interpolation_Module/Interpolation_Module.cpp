#include "../../include/Interpolation_Module/Interpolation.hpp"
#include "../../include/Interpolation_Module/small_classes.hpp"
#include <set>
#include <algorithm>
#include <stdexcept>
#include <limits>

namespace ScientificToolbox {

    template <typename T>
    class LinearInterpolation : public Interpolation<T> {
    public:
        // Constructor accepts a set of points
        LinearInterpolation(const typename Interpolation<T>::set& data);

        // Overload operator()
        T operator()(T x) const;

    private:
        std::vector<std::pair<interval<T>, abline<T>>> interpolated_functions;
    };

    // Constructor accepts a set of points
    template <typename T>
    LinearInterpolation<T>::LinearInterpolation(const typename Interpolation<T>::set& data)
        : Interpolation<T>(data)  // Pass the set directly to the base class
    {
        // Iterate over the set to create intervals and ablines
        auto it = data.begin();
        auto prev_it = it;
        ++it;  // Move to the next element

        // Iterate over the set to create intervals and ablines
        while (it != data.end()) {
            interval<T> intv(prev_it->get_x(), it->get_x());  // Create an interval between x-values
            abline<T> abline(*prev_it, *it);  // Create an abline between consecutive points
            interpolated_functions.push_back(std::make_pair(intv, abline));

            prev_it = it;
            ++it;
        }
    }


    // Operator () implementation
    template <typename T>
    T LinearInterpolation<T>::operator()(T x) const {
        // Find the correct interval for the given x
        auto it = std::find_if(interpolated_functions.begin(), interpolated_functions.end(),
            [x](const std::pair<interval<T>, abline<T>>& p) { return p.first.contains(x); });

        // If no interval contains x, return NaN
        if (it == interpolated_functions.end()) {
            return std::numeric_limits<T>::quiet_NaN();
        }

        // Evaluate the corresponding abline at x
        return it->second.evaluate(x);
    }

}
