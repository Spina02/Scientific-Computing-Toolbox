
#ifndef ODESOLVER_HPP
#define ODESOLVER_HPP

#include <vector>
#include <functional>
#include "../Utilities.hpp"

namespace ScientificToolbox {
    
    class ODESolver {
    public:
        ODESolver() = default;
        virtual ~ODESolver() = default;

        virtual vec_d Solve(std::function<vec_d(double, vec_d)> f, double t0, vec_d y0, double t1, double h) = 0;

    };
}

#endif