#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include <chrono>
#include "ODESolver.hpp"
#include "utils.hpp"

namespace ScientificToolbox::ODE {

// Computes the error between the result and the expected value
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
        throw std::runtime_error("Tipo di variante incompatibile per il calcolo dell'errore.");
    }
}

// Mesure time taken to run a function
ODESolution solve_and_measure_execution_time(const ODESolver& solver) {
    auto start = std::chrono::high_resolution_clock::now();
    ODESolution sol = solver.Solve(); // Call the Solve() method
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Elapsed Time: " << diff.count() << std::endl; // Time in seconds
    return sol;
}

} // namespace ScientificToolbox::ODE

#endif // ANALYSIS_HPP