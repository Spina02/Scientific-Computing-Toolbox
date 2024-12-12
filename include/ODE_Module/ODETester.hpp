#ifndef ODETESTER_HPP
#define ODETESTER_HPP

/**
 * @file ODETester.hpp
 * @brief Testing framework for ODE solvers
 * 
 * This module provides a testing framework for the ODE solver implementations
 * and the mathematical expression parser.
 */

#include "ODESolver.hpp"
#include "ForwardEulerSolver.hpp"
#include "RK4Solver.hpp"
#include "ExplicitMidpointSolver.hpp"
#include "analysis.hpp"
#include "../Utilities/ImportCSV.hpp"
#include <iostream>

/**
 * @namespace ScientificToolbox::ODE
 * @brief Namespace containing utilities for Ordinary Differential Equations (ODE) handling
*/
namespace ScientificToolbox::ODE {

/**
 * @class ODETester
 * @brief Testing framework for ODE solvers
 * 
 * Provides functionality to test both the expression parser and various ODE solvers
 * against known solutions.
 */
class ODETester {
public:
    ODETester();
    ~ODETester() = default;

    /** ### run_parser_tests
     * @brief Tests the mathematical expression parser
     * @return true if all tests pass, false otherwise
     */
    bool run_parser_tests() const;

    /** ### run_ode_tests
     * @brief Tests the ODE solver implementations
     * @return true if all tests pass, false otherwise
     */
    bool run_ode_tests() const;

private:
    /** ### test_expression
     * @brief Run a single test case for the expression parser
     * @param expr_variant Variant containing the expression to test
     * @param t_val Time value for the expression
     * @param y_val Vector of values for the expression
     * @param expected_val Expected result of the expression
     * @param test_num Test case number
     * @return true if the test passes, false otherwise
     */
    bool test_expression(ODETestCase test, int test_num) const;
    
    /** ### test_simple_ode
     * @brief Run a single test case for an ODE solver
     * @param test_case Test case to run
     * @param solver_type Type of solver to use
     * @param test_num Test case number
     * @return true if the test passes, false otherwise
     */
    bool test_simple_ode(const ODETestCase& test_case, const std::string solver_type, int test_num) const;
};

} // namespace ScientificToolbox::ODE

#endif // ODETESTER_HPP