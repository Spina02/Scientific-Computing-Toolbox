#ifndef RK4SOLVER_HPP
#define RK4SOLVER_HPP

#include "ODESolver.hpp"

namespace ScientificToolbox::ODE {

class RK4Solver : public ODESolver{
    public:
        // Inherit constructor from parent class
        using ODESolver::ODESolver;
        // Default destructor
        virtual ~RK4Solver() = default;
        
        // Solve the ODE using RK4 method
        virtual ODESolution Solve() const override;
};

} // ScientificToolbox::ODE

#endif // RK4SOLVER_HPP