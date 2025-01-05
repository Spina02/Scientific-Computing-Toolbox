#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include "utils.hpp"
#include "ODESolver.hpp"
#include "ForwardEulerSolver.hpp"
#include "ExplicitMidpointSolver.hpp"
#include "RK4Solver.hpp"
#include <chrono>
#include <map>
#include <memory>

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

/** ### TimedSolution
 * @brief Struct to store a solution with its execution time
 */
struct TimedSolution {
    ODESolution solution;
    double elapsed_time;
};

/** ### compute_error
 * @brief Computes the error between numerical and analytical solutions
 * @param result Numerical solution
 * @param expected Analytical solution
 * @param sensitivity Error sensitivity threshold
 * @return Computed error value
 */
double compute_error(const var_vec& result, const var_vec& expected);

/** ### solve_and_measure_execution_time
 * @brief Measures execution time of ODE solver
 * @param solver The ODE solver to test
 * @return Solution with execution time printed to stdout
 */
TimedSolution solve_and_measure_execution_time(const ODESolver& solver);

/** ### compute_order_of_convergence
 * @brief Compute the order of convergence for a given solver
 * @param solver_type Type of solver to analyze
 * @return Order of convergence
 */
double compute_order_of_convergence(std::string solver_type);

using SolverFactory = std::function<std::unique_ptr<ODESolver>(Func f, double y0, double t0, double tf, double h)>;

extern std::map<std::string, SolverFactory> factories;

} // namespace ScientificToolbox::ODE

#endif // ANALYSIS_HPP