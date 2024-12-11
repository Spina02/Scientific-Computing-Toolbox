#ifndef FORWARDEULERSOLVER_HPP
#define FORWARDEULERSOLVER_HPP

/**
 * @file ForwardEulerSolver.hpp
 * @brief Implements the Forward Euler method for ODE solving
 * 
 * This module provides the Forward Euler method for solving ordinary differential equations.
 */

#include "ODESolver.hpp"

/**
 * @namespace ScientificToolbox::ODE
 * @brief Namespace containing utilities for Ordinary Differential Equations (ODE) handling
 */
namespace ScientificToolbox::ODE {

/**
 * @class ForwardEulerSolver
 * @brief Implements the Forward Euler method for ODE solving
 * 
 * Provides a first-order numerical solution using the explicit Euler method.
 */
class ForwardEulerSolver : public ODESolver {
    public:
        // Inherit constructor from parent class
        using ODESolver::ODESolver;
        // Default destructor
        virtual ~ForwardEulerSolver() = default;

        /** ### Solve
         * @brief Solves the ODE using the Forward Euler method
         * @return ODESolution containing the solution data
         */
        virtual ODESolution Solve() const override;
};

} // namespace ScientificToolbox::ODE

#endif // FORWARDEULERSOLVER_HPP