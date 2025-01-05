#include "../../include/ODE_Module/analysis.hpp"

namespace ScientificToolbox::ODE {

std::map<std::string, SolverFactory> factories = {
    {"ForwardEulerSolver", [](Func f, double y0, double t0, double tf, double h) {
        return std::make_unique<ForwardEulerSolver>(f, y0, t0, tf, h);
    }},
    {"ExplicitMidpointSolver", [](Func f, double y0, double t0, double tf, double h) {
        return std::make_unique<ExplicitMidpointSolver>(f, y0, t0, tf, h);
    }},
    {"RK4Solver", [](Func f, double y0, double t0, double tf, double h) {
        return std::make_unique<RK4Solver>(f, y0, t0, tf, h);
    }}
};

double compute_error(const var_vec& result, const var_vec& expected) {
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

TimedSolution solve_and_measure_execution_time(const ODESolver& solver) {
    auto start = std::chrono::high_resolution_clock::now();
    ODESolution sol = solver.solve(); // Call the solve() method
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return {sol, diff.count()};
}

double compute_order_of_convergence(std::string solver_type) {
    Func f {parseExpression("y")}; // Dummy function
    double solution = std::exp(1); // Dummy solution
    double t0 = 0;
    double tf = 1;
    double y0 = 1;
    vec_d steps(4);
    steps << 1/8., 1/16., 1/32., 1/64.;
    vec_d errors(steps.size());

    for (Eigen::Index i = 0; i < steps.size(); ++i) {
        double h = steps[i];
        // Create solver using unique_ptr and keep it in scope
        auto current_solver = factories[solver_type](f, y0, t0, tf, h);
        ODESolution sol = current_solver->solve();
        const auto& result = sol.get_result();
        double error = compute_error(result, solution);
        errors(i) = error;
    }

    // Compute order of convergence
    vec_d orders(steps.size() - 1);
    for (Eigen::Index i = 0; i < steps.size() - 1; ++i) {
        if (steps(i) <= 0.0 || errors(i) <= 0.0) {
            throw std::invalid_argument("Stepsizes and errors must be positive to compute order of convergence.");
        }
        double log_h_ratio = std::log(steps(i) / steps(i + 1));
        double log_error_ratio = std::log(errors(i) / errors(i + 1));
        orders(i) = log_error_ratio / log_h_ratio;
    }
    return orders.mean();
}

} // namespace ScientificToolbox::ODE