#ifndef LINEARINTERPOLATION_HPP
#define LINEARINTERPOLATION_HPP

#include "../../include/Interpolation_Module/Interpolation.hpp"
#include "../../include/Interpolation_Module/small_classes.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename T>
class LinearInterpolation : public Interpolation<T> {
public:
    LinearInterpolation(const std::vector<point<T>>& data);

    // Overload operator()
    T operator()(T x) const;

private:
    std::vector<std::pair<interval<T>, abline<T>>> interpolated_functions;
};

// Constructor implementation
template <typename T>
LinearInterpolation<T>::LinearInterpolation(const std::vector<point<T>>& data) : Interpolation<T>(data) {
    std::vector<point<T>> sorted_data = data;
    std::sort(sorted_data.begin(), sorted_data.end(), 
        [](const point<T>& p1, const point<T>& p2) { return p1.get_x() < p2.get_x(); });

    for (size_t i = 0; i < sorted_data.size() - 1; ++i) {
        interval<T> interval(sorted_data[i].get_x(), sorted_data[i + 1].get_x());
        abline<T> abline(sorted_data[i], sorted_data[i + 1]);
        interpolated_functions.push_back(std::make_pair(interval, abline));
    }
}

// Operator () implementation
template <typename T>
T LinearInterpolation<T>::operator()(T x) const {
    auto it = std::find_if(interpolated_functions.begin(), interpolated_functions.end(), 
        [x](const std::pair<interval<T>, abline<T>>& p) { return p.first.contains(x); });

    if (it == interpolated_functions.end()) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    return it->second.evaluate(x);
}

#endif
