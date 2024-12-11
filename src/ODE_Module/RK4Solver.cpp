#include "../../include/ODE_Module/RK4Solver.hpp"

namespace ScientificToolbox::ODE {

// Implementation of Runge-Kutta-4 Solever
ODESolution RK4Solver::Solve() const {

    if (h <= 0) throw std::invalid_argument("Step size h must be positive.");
    if (t0 >= tf) throw std::invalid_argument("Initial time t0 must be less than final time tf.");

    ODESolution solution;
    double t = t0;
    var_vec y = y0;
    int n = static_cast<int> ((tf-t0)/h);
    solution.size = n;
    
    // Store initial conditions
    solution.y_values.reserve(n + 1);
    solution.y_values.push_back(y);
    solution.t_values.conservativeResize(solution.t_values.size() + 1);
    solution.t_values(solution.t_values.size() - 1) = t0;

    try {
        while (t < tf) {
            
            var_vec y_next = y;  // Create new variable for next step
            
            var_vec k_1 = f(t, y);
            var_vec k_2 = f(t + h/2, y + h*k_1/2.0);
            var_vec k_3 = f(t + h/2, y + h*k_2/2.0);
            var_vec k_4 = f(t + h, y + h*k_3);

            y_next = y + h/6*(k_1 + 2*k_2 + 2*k_3 + k_4);

            t += h;
            y = y_next;

            solution.t_values.conservativeResize(solution.t_values.size() + 1);
            solution.t_values(solution.t_values.size() - 1) = t;
            solution.y_values.push_back(y);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error in RK4Solver::Solve: ") + e.what());
    }
    return solution;
}

} // ScientficToolbox::ODE