#ifndef ODESOLVER_HPP
#define ODESOLVER_HPP

/**
 * @file ODESolver.hpp
 * @brief Abstract base class for ODE numerical solvers
 * 
 * This module defines the interface for all ODE solver implementations.
 * It provides:
 * - Common initialization for solver parameters
 * - Virtual interface for solving ODEs
 * - Basic data structures for solution handling
 */

#include "utils.hpp"
#include <vector>
#include <variant>

/**
 * @namespace ScientificToolbox::ODE
 * @brief Namespace containing utilities for Ordinary Differential Equations (ODE) handling
 */
namespace ScientificToolbox::ODE {
    
/**
 * @class ODESolver
 * @brief Abstract base class for ODE numerical solvers
 * 
 * Provides interface and common functionality for implementing
 * specific ODE solving methods.
 */
class ODESolver {    
public:
    // Constructors
    ODESolver(const var_expr ex,  const var_vec& y0, double t0, double tf, double h) : expr(ex), f(parseExpression(ex), ex), t0(t0), tf(tf), h(h), y0(y0) { }
    ODESolver(const ODETestCase& test) : f(parseExpression(test.expr), test.expr), t0(test.t0), tf(test.tf), h(test.h), y0(test.y0), expr(test.expr) { }

    // Default destructor
    virtual ~ODESolver() = default;

    /** ### Solve
     * @brief Solve the ODE using the implemented method
     * @return ODESolution containing the solution data
     */
    virtual ODESolution solve() const = 0; 

protected:
    // State variables
    var_expr expr;
    Func f;
    double t0;
    double tf;
    double h;
    var_vec y0;
};

} // namespace ScientificToolbox::ODE

#endif // ODESOLVER_HPP