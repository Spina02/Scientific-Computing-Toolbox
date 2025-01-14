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
#include "../Utilities.hpp"
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
    ODETester() = default;
    ODETester(std::string filename);

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
    /** ### almost_equal
     * @brief Check if two var_vecs are almost equal
     * @param a First vector
     * @param b Second vector
     * @param tol Tolerance for comparison
     * @return true if the vectors are almost equal, false otherwise
     */
    bool almost_equal(const var_vec& a, const var_vec& b, double tol = 1e-10) const {
        return std::visit([tol](const auto& x, const auto& y) -> bool {
            using T1 = std::decay_t<decltype(x)>;
            using T2 = std::decay_t<decltype(y)>;

            // If types don't match, they're not equal
            if constexpr (!std::is_same_v<T1, T2>) {
                return false;
            }
            // Both are doubles
            else if constexpr (std::is_same_v<T1, double>) {
                return std::abs(x - y) <= tol;
            }
            // Both are vectors
            else {
                if (x.size() != y.size()) return false;
                // Change size_t to Eigen::Index to match vector size type
                for (Eigen::Index i = 0; i < x.size(); ++i) {
                    if (std::abs(x[i] - y[i]) > tol) return false;
                }
                return true;
            }
        }, a, b);
    }
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