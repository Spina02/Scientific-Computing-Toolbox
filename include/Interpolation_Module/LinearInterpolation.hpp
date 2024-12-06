#ifndef LINEARINTERPOLATION_HPP
#define LINEARINTERPOLATION_HPP

#include "Interpolation.hpp"
#include "small_classes.hpp"

#include <set>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace ScientificToolbox {

    // Derived class for linear interpolation
    template <typename T>
    class LinearInterpolation : public Interpolation<T> {
    public:
        LinearInterpolation(const set<T>& data);

        // Overload operator()
        T operator()(T x) const override;

        // Implement interpolate
        T interpolate(T x) const override;

    private:
        std::vector<std::pair<interval<T>, abline<T>>> interpolated_functions;
    };
}

#endif
