#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "small_classes.hpp"
#include <set>
#include <stdexcept>

namespace ScientificToolbox {

    // Abstract base class for interpolation
    template <typename T>
    class Interpolation {
    public:
        using set = std::set<point<T>>; // Move alias inside the class

        // Constructor
        explicit Interpolation(const set& data) : data(data) {
            if (data.empty()) {
                throw std::invalid_argument("Data points cannot be empty.");
            }
        }

        // Virtual destructor
        virtual ~Interpolation() = default;

        // Pure virtual interpolation function
        virtual T interpolate(T x) const = 0;

        // Overload operator()
        virtual T operator()(T x) const { return interpolate(x); }

    protected:
        // Getter for data
        const set& getData() const { return data; }

    private:
        set data; // Stored interpolation data
    };

} // namespace ScientificToolbox

#endif
