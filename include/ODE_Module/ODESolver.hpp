#ifndef ODESOLVER_HPP
#define ODESOLVER_HPP

#include "utils.hpp"
#include <vector>
#include <variant>

namespace ScientificToolbox::ODE {
    
    class ODESolver {    
    public:
        ODESolver(const Func& f, double t0, const var_vec& y0, double tf, double h)
            : f(f), t0(t0), tf(tf), h(h), y0(y0) {}

        virtual ~ODESolver() = default;

        virtual ODESolution Solve() const = 0; 

    protected:
        Func f;
        double t0;
        double tf;
        double h;
        var_vec y0;
        };
}

#endif // ODESOLVER_HPP