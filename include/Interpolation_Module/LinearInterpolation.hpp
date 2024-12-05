#ifndef LINEARINTERPOLATION_HPP
#define LINEARINTERPOLATION_HPP

#include "Interpolation.hpp"
#include "small_classes.hpp"

#include <vector>
#include <algorithm>
#include <stdexcept>

// Derived class for linear interpolation
template <typename T>
class LinearInterpolation : public Interpolation<T> {
public:
    LinearInterpolation(const std::vector<point<T>>& data);

    // Overload operator()
    T operator()(T x) const override;

    // Implement interpolate
    T interpolate(T x) const override;

private:
    std::vector<std::pair<interval<T>, abline<T>>> interpolated_functions;
};

// Constructor
template <typename T>
LinearInterpolation<T>::LinearInterpolation(const std::vector<point<T>>& data) 
    : Interpolation<T>(data) {
    // Create intervals and corresponding ablines
    for (size_t i = 0; i < data.size() - 1; ++i) {
        abline<T> ab(data[i], data[i + 1]);
        interval<T> intv(data[i].get_x(), data[i + 1].get_x());
        interpolated_functions.push_back(std::make_pair(intv, ab));
    }
}

// Implementing interpolate
template <typename T>
T LinearInterpolation<T>::interpolate(T x) const {
    // Use binary search to find the correct interval
    auto it = std::lower_bound(
        interpolated_functions.begin(),
        interpolated_functions.end(),
        x,
        [](const std::pair<interval<T>, abline<T>>& func, T value) {
            return func.first.get_upper_bound() < value;
        }
    );

    if (it == interpolated_functions.end() || !it->first.contains(x)) {
        throw std::out_of_range("x is out of the interpolation range.");
    }

    return it->second.evaluate(x);
}

// Overload operator()
template <typename T>
T LinearInterpolation<T>::operator()(T x) const {
    return interpolate(x);
}

#endif
