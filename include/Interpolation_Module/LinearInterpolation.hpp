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
        using typename Interpolation<T>::set;

        LinearInterpolation(const set& data) : Interpolation<T>(data) {
            // Iterate over the set to create intervals and ablines
            auto it = data.begin();
            auto prev_it = it;
            ++it;

            while (it != data.end()) {
                interval<T> intv(prev_it->get_x(), it->get_x());
                abline<T> ab(*prev_it, *it);
                interpolated_functions.push_back(std::make_pair(intv, ab));

                prev_it = it;
                ++it;
            }
        }

        // Implement the interpolate method
        T interpolate(T x) const override {
            auto it = std::find_if(interpolated_functions.begin(), interpolated_functions.end(),
                [x](const std::pair<interval<T>, abline<T>>& p) { return p.first.contains(x); });

            if (it == interpolated_functions.end()) {
                throw std::out_of_range("Value is outside the interpolation range.");
            }

            return it->second.evaluate(x);
        }

        // Overload operator()
        T operator()(T x) const override {
            return interpolate(x);
        }

    private:
        std::vector<std::pair<interval<T>, abline<T>>> interpolated_functions;
    };

}

#endif
