#ifndef EXPLICITMIDPOINTSOLVER_HPP
#define EXPLICITMIDPOINTSOLVER_HPP

#include "ODESolver.hpp"

namespace ScientificToolbox::ODE {

class ExplicitMidpointSolver : public ODESolver {
    public:
        // Inherit constructor from parent class
        using ODESolver::ODESolver;
        // Default destructor
        virtual ~ExplicitMidpointSolver() = default;

        // Solve the ODE using Explicit Midpoint method
        virtual std::vector<var_vec> Solve() const override;
};

} // ScientificToolbox::ODE

#endif // EXPLICITMIDPOINTSOLVER_HPP
