#ifndef POLYNOMIALINTERPOLATION_HPP
#define POLYNOMIALINTERPOLATION_HPP

namespace ScientificToolbox{
    
    template <typename T>
    class PolynomialInterpolation() : public Interpolation<T>{

    };
    
    template <typename T>
    class Lagrange() : public PolynomialInterpolation<T>{

    };

    template <typename T>
    class Newton() : public PolynomialInterpolation<T>{

    };
}

#endif