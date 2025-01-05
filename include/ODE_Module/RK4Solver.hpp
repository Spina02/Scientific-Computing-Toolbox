#ifndef RK4SOLVER_HPP
#define RK4SOLVER_HPP

/**
 * @file RK4Solver.hpp
 * @brief Implements the 4th order Runge-Kutta method for ODE solving
 * 
 * This module provides the 4th order Runge-Kutta method for solving ordinary differential equations.
 */

#include "ODESolver.hpp"

/**
 * @namespace ScientificToolbox::ODE
 * @brief Namespace containing utilities for Ordinary Differential Equations (ODE) handling
 */
namespace ScientificToolbox::ODE {

/**
 * @class RK4Solver
 * @brief Implements the 4th order Runge-Kutta method for ODE solving
 * 
 * The RK4 method provides a higher-order accurate solution compared to simpler methods.
 */
class RK4Solver : public ODESolver{
    public:
        // Inherit constructor from parent class
        using ODESolver::ODESolver;
        // Default destructor
        virtual ~RK4Solver() = default;
        
        /** ### Solve
         * @brief Solve the ODE using 4th order Runge-Kutta method
         * @return ODESolution containing the solution data
         */
        virtual ODESolution solve() const override;
};

} // namespace ScientificToolbox::ODE

#endif // RK4SOLVER_HPP