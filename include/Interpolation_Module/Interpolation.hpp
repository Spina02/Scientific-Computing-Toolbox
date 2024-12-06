#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "small_classes.hpp"
#include <set>
#include <stdexcept>

namespace ScientificToolbox {

    // Alias for set of points
    template <typename T>
    using set = std::set<point<T>>; // Make sure to use consistent naming

    // Abstract base class
    template <typename T>
    class Interpolation {
    public:
        Interpolation(const set<T>& data) : data(data) {
            if (data.empty()) {
                throw std::invalid_argument("Data points cannot be empty.");
            }
        }

        virtual ~Interpolation() = default;

        virtual T interpolate(T x) const = 0;
        virtual T operator()(T x) const { return interpolate(x); }

    protected:
        const set<T>& getData() const { return data; }

    private:
        set<T> data;
    };
}

#endif
