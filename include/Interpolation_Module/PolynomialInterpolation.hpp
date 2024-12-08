#ifndef POLYNOMIALINTERPOLATION_HPP
#define POLYNOMIALINTERPOLATION_HPP

#include "Interpolation.hpp"
#include <vector>
#include <set>  // Needed for std::set

/**
 * namespace ScientificToolbox::Interpolation
 * @brief Interpolation module providing basic interpolation functions
 * 
 * This namespace contains template classes for common interpolation methods:
 * - Linear interpolation
 * - Polynomial interpolation
 * - Spline interpolation
 */

/** PolynomialInterpolation class
 * @brief Abstract class for polynomial interpolation
 * 
 * This class is an abstract class for polynomial interpolation. It is a subclass of the Interpolation class.
 * It contains the x and y values of the data points and provides a pure virtual function for interpolation.
 *  
 * @tparam T Data type of x and y values
 * 
 * @see Interpolation
 * 
 */

/** Constructor
 * @brief Constructor for PolynomialInterpolation class
 * 
 * This constructor obtains the x and y values from the data points using the toVectors() method defined in the Interpolation class.
 * It throws an exception if the data points are empty.
 * 
 * @param data Data points
 * 
 * @see Interpolation::toVectors()
 * 
 */

/** Destructor
 * @brief Virtual destructor for PolynomialInterpolation class
 * 
 * This is a virtual destructor for the PolynomialInterpolation class.
 * 
 */

/** interpolate
 * @brief Pure virtual function for polynomial interpolation
 * 
 * This is a pure virtual function for polynomial interpolation. It takes an x value as input and returns the interpolated y value.
 * 
 * @param x x value
 * 
 * @return T Interpolated y value
 * 
 */

/** operator()
 * @brief Overload operator() for polynomial interpolation
 * 
 * This is an overloaded operator() for polynomial interpolation. It takes an x value as input and returns the interpolated y value.
 *  
 * @param x x value
 * 
 * @return T Interpolated y value
 * 
 */

namespace ScientificToolbox {

    template <typename T>
    class PolynomialInterpolation : public Interpolation<T> {
    public:
        // Constructor
        explicit PolynomialInterpolation(const typename Interpolation<T>::point_set& data) : Interpolation<T>(data) {
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
