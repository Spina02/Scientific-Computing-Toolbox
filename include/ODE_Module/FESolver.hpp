#ifndef FORWARDEULERSOLVER_HPP
#define FORWARDEULERSOLVER_HPP

#include "ODESolver.hpp"
#include "../Utilities.hpp"

namespace ScientificToolbox {
    class ForwardEulerSolver : public ODESolver {
    public:
        // Inherit constructor from parent class
        using ODESolver::ODESolver;
        
        virtual ~ForwardEulerSolver() = default;
        virtual std::vector<var_vec> Solve() override;
    };
}

#endif // FORWARDEULERSOLVER_HPP