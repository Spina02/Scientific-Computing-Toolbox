#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include "utils.hpp"
#include "ODESolver.hpp"
#include <chrono>

/**
 * @file analysis.hpp
 * @brief Analysis utilities for ODE solvers
 * 
 * This module provides utility functions for analyzing the performance of ODE solvers,
 * including:
 * - Error computation between numerical and analytical solutions
 * - Execution time measurement
 */

namespace ScientificToolbox::ODE {

var_vec abs(const var_vec& v) {
    if (std::holds_alternative<double>(v)) {
        return std::abs(std::get<double>(v));
    } else if (std::holds_alternative<vec_d>(v)) {
        return (std::get<vec_d>(v)).cwiseAbs();
    } else {
        throw std::runtime_error("Error: Incompatible types for abs function");
    }
}

/** ### compute_error
 * @brief Computes the error between numerical and analytical solutions
 * @param result Numerical solution
 * @param expected Analytical solution
 * @param sensitivity Error sensitivity threshold
 * @return Computed error value
 */
double compute_error(const var_vec& result, const var_vec& expected, double sensitivity) {
    if (std::holds_alternative<double>(result) && std::holds_alternative<double>(expected)) {
        double num = std::get<double>(result);
        double ana = std::get<double>(expected);
        return std::abs(num - ana);
    } else if (std::holds_alternative<vec_d>(result) && std::holds_alternative<vec_d>(expected)) {
        const vec_d& num = std::get<vec_d>(result);
        const vec_d& ana = std::get<vec_d>(expected);
        return (num - ana).norm();
    } else {
        throw std::runtime_error("Error: Incompatible types for error computation");
    }
}

/** ### solve_and_measure_execution_time
 * @brief Measures execution time of ODE solver
 * @param solver The ODE solver to test
 * @return Solution with execution time printed to stdout
 */
ODESolution solve_and_measure_execution_time(const ODESolver& solver) {
    auto start = std::chrono::high_resolution_clock::now();
    ODESolution sol = solver.Solve(); // Call the Solve() method
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "  Elapsed Time: " << diff.count() << std::endl; // Time in seconds
    return sol;
}

} // namespace ScientificToolbox::ODE

#endif // ANALYSIS_HPP