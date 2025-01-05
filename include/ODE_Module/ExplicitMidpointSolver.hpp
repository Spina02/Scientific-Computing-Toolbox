#ifndef EXPLICITMIDPOINTSOLVER_HPP
#define EXPLICITMIDPOINTSOLVER_HPP

/**
 * @file ExplicitMidpointSolver.hpp
 * @brief Implements the Explicit Midpoint method for ODE solving
 * 
 * This module provides the Explicit Midpoint method for solving ordinary differential equations.
 */

#include "ODESolver.hpp"

/**
 * @namespace ScientificToolbox::ODE
 * @brief Namespace containing utilities for Ordinary Differential Equations (ODE) handling
 */
namespace ScientificToolbox::ODE {

/**
 * @class ExplicitMidpointSolver
 * @brief Implements the Explicit Midpoint method for ODE solving
 * 
 * A second-order Runge-Kutta method that provides better accuracy than Euler method.
 */
class ExplicitMidpointSolver : public ODESolver {
    public:
        // Inherit constructor from parent class
        using ODESolver::ODESolver;
        // Default destructor
        virtual ~ExplicitMidpointSolver() = default;

        /** ### Solve
         * @brief Solves the ODE using the Explicit Midpoint method
         * @return ODESolution containing the solution data
         */
        virtual ODESolution solve() const override;
};

} // namespace ScientificToolbox::ODE

#endif // EXPLICITMIDPOINTSOLVER_HPP
