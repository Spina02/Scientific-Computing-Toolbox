#ifndef FORWARDEULERSOLVER_HPP
#define FORWARDEULERSOLVER_HPP

#include "ODESolver.hpp"

namespace ScientificToolbox::ODE {

class ForwardEulerSolver : public ODESolver {
    public:
        // Inherit constructor from parent class
        using ODESolver::ODESolver;
        // Default destructor
        virtual ~ForwardEulerSolver() = default;

        // Solve the ODE using Forward Euler method
        virtual std::vector<var_vec> Solve() override;
};

} // ScientificToolbox::ODE

#endif // FORWARDEULERSOLVER_HPP