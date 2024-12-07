#ifndef ODESOLVER_HPP
#define ODESOLVER_HPP

#include "../Utilities.hpp"
#include <vector>
#include <variant>

namespace ScientificToolbox {
    
    class ODESolver {
    // State variables
    protected:
        var_func f;
        double t0;
        double tf;
        double h;
        var_vec y0;
    // Member functions
    public:
        ODESolver(var_func f, double t0, var_vec y0, double tf, double h)
            : f(f), t0(t0), tf(tf), h(h), y0(y0) {}

        virtual ~ODESolver() = default;

        virtual std::vector<var_vec> Solve() = 0; 
    };
}

#endif // ODESOLVER_HPP