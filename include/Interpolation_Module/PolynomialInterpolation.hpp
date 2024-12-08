#ifndef POLYNOMIALINTERPOLATION_HPP
#define POLYNOMIALINTERPOLATION_HPP

namespace ScientificToolbox {

    #include "Interpolation.hpp"
    #include <vector>
    #include <set>  // Needed for std::set

    template <typename T>
    class PolynomialInterpolation : public Interpolation<T> {
    public:
        // Constructor
        explicit PolynomialInterpolation(const typename Interpolation<T>::set& data) : Interpolation<T>(data) {
            if (data.empty()) {
                throw std::invalid_argument("Data points cannot be empty.");
            }
            // Obtaining x and y from data using toVectors() method
            std::pair<std::vector<T>, std::vector<T>> vector_xy = this->toVectors();
            x = vector_xy.first;
            y = vector_xy.second;
        }

        // Virtual Destructor
        virtual ~PolynomialInterpolation() = default;

        // Pure virtual interpolation function
        virtual T interpolate(T x) const = 0;

        // Overload operator()
        virtual T operator()(T x) const { return interpolate(x); }

    protected:
        std::vector<T> x; // x values
        std::vector<T> y; // y values
    };

}

#endif
