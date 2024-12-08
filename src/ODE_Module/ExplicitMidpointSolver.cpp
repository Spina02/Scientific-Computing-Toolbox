#include "../../include/ODE_Module/ExplicitMidpointSolver.hpp"

namespace ScientificToolbox::ODE {

std::vector<var_vec> ExplicitMidpointSolver::Solve() {
    
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

            if (auto* scalar_f = std::get_if<scalar_func>(&f)) {
                if (auto* scalar_y = std::get_if<double>(&y)) {

                    double k_1 = (*scalar_f)(t, *scalar_y);
                    double k_2 = (*scalar_f)(t + h/2, *scalar_y + h*k_1/2);
                    y_next = (*scalar_y) + h * k_2;
                
                } else {
                    throw std::runtime_error("Mismatched scalar function with vector input");
                }
            } else if (auto* vector_f = std::get_if<vec_func>(&f)) {
                if (auto* vector_y = std::get_if<vec_d>(&y)) {

                    vec_d k_1 = (*vector_f)(t, *vector_y);
                    vec_d k_2 = (*vector_f)(t + h/2, *vector_y + h*k_1/2);
                    y_next = *vector_y + h * k_2;
                
                } else {
                    throw std::runtime_error("Mismatched vector function with scalar input");
                }
            }

            t += h;
            y = y_next;
            result.push_back(y);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error in FESolver::Solve: ") + e.what());
    }
    return result;
}

} // namespace ScientificToolbox