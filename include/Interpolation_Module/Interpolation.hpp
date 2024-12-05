#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "small_classes.hpp"

#include <vector>
#include <stdexcept>


// Abstract base class
template <typename T>
class Interpolation {
public:
    Interpolation(const std::vector<point<T>>& data) : data(data) {
        if (data.empty()) {
            throw std::invalid_argument("Data points cannot be empty.");
        }
    }

    virtual ~Interpolation() = default;

    virtual T interpolate(T x) const = 0;
    virtual T operator()(T x) const { return interpolate(x); }

protected:
    const std::vector<point<T>>& getData() const { return data; }

private:
    std::vector<point<T>> data;
};

#endif