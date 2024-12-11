#include "../../include/ODE_Module/ForwardEulerSolver.hpp"

namespace ScientificToolbox::ODE {

// Implementation of Forward Euler Solver
std::vector<var_vec> ForwardEulerSolver::Solve() const {

    if (h <= 0) throw std::invalid_argument("Step size h must be positive.");
    if (t0 >= tf) throw std::invalid_argument("Initial time t0 must be less than final time tf.");

    std::vector<var_vec> result;
    double t = t0;
    var_vec y = y0;
    int n = static_cast<int>((tf - t0) / h);
    result.reserve(n + 1);
    result.push_back(y);  // Store initial condition

    try {
        while (t < tf) {
            var_vec y_next = y;  // Create new variable for next step

            y_next = y + h * f(t, y);

            t += h;
            y = y_next;
            result.push_back(y);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error in FESolver::Solve: ") + e.what());
    }
    return result;
}

} // ScientificToolbox::ODE