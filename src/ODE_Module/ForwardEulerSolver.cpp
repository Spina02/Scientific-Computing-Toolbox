#include "../../include/ODE_Module/ForwardEulerSolver.hpp"

namespace ScientificToolbox::ODE {

// Implementation of Forward Euler Solver
ODESolution ForwardEulerSolver::solve() const {
    if (h <= 0) throw std::invalid_argument("Step size h must be positive.");
    if (t0 >= tf) throw std::invalid_argument("Initial time t0 must be less than final time tf.");

    ODESolution solution;

    solution.expr = expr;

    double t = t0;
    var_vec y = y0;
    int n = static_cast<int>((tf - t0) / h);

    // Determine size of the solution (= system rank)
    solution.size = std::visit([](const auto& val) -> size_t {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, double>) {
            return 1;
        } else {
            return val.size();
        }
    }, y);
    
    solution.y_values.reserve(n + 1);
    // Store initial conditions
    solution.y_values.reserve(n + 1);
    solution.y_values.push_back(y);
    solution.t_values.conservativeResize(solution.t_values.size() + 1);
    solution.t_values(solution.t_values.size() - 1) = t0;

    try {
        for(int i = 0; i < n; ++i) {
            var_vec y_next = y;  // Create new variable for next step

            y_next = y + h * f(t, y);

            t += h;
            y = y_next;
            solution.y_values.push_back(y);
            solution.t_values.conservativeResize(solution.t_values.size() + 1);
            solution.t_values(solution.t_values.size() - 1) = t;
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error in FESolver::Solve: ") + e.what());
    }
    return solution;
}

} // ScientificToolbox::ODE