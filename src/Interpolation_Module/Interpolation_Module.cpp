#include "../../include/Interpolation_Module.hpp"

LinearInterpolation::LinearInterpolation(const std::vector<point>& data) 
    : Interpolation(data) {
    // Create intervals and their corresponding lines
    for (size_t i = 0; i < data.size() - 1; ++i) {
        abline ab(data[i], data[i + 1]);
        interval intv(data[i].get_x(), data[i + 1].get_x());
        interpolated_functions.push_back(std::make_pair(intv, ab));
    }
}

double LinearInterpolation::interpolate(double x) const {
    // Use binary search to find the correct interval
    auto it = std::lower_bound(
        interpolated_functions.begin(), 
        interpolated_functions.end(), 
        x,
        [](const std::pair<interval, abline>& func, double value) {
            return func.first.get_upper_bound() < value;
        }
    );

    if (it == interpolated_functions.end() || !it->first.contains(x)) {
        throw std::out_of_range("x is out of the interpolation range.");
    }

    return it->second.evaluate(x);
}
