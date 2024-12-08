#include "../../include/ODE_Module/RK4Solver.hpp"

namespace ScientificToolbox::ODE {

// Implementation of Runge-Kutta-4 Solever
std::vector<var_vec> RK4Solver::Solve() {

    if (h <= 0) throw std::invalid_argument("Step size h must be positive.");
    if (t0 >= tf) throw std::invalid_argument("Initial time t0 must be less than final time tf.");

    std::vector<var_vec> result;
    double t = t0;
    var_vec y = y0;
    int n = static_cast<int> ((tf-t0)/h);
    result.reserve(n+1);
    result.push_back(y);

    try {
        while (t < tf) {
            var_vec y_next = y;  // Create new variable for next step
            
            if (auto* scalar_f = std::get_if<scalar_func>(&f)) {
                if (auto* scalar_y = std::get_if<double>(&y)) {
                    double k_1 = (*scalar_f)(t, *scalar_y);
                    double k_2 = (*scalar_f)(t + h/2, *scalar_y + k_1*h/2);
                    double k_3 = (*scalar_f)(t + h/2, *scalar_y + k_2*h/2);
                    double k_4 = (*scalar_f)(t +h, *scalar_y + k_3*h);
                    y_next = *scalar_y + h/6*(k_1 + 2*k_2 + 2*k_3 + k_4);
                } else {
                    throw std::runtime_error("Mismatched scalar function with vector input");
                }
            } else if (auto* vector_f = std::get_if<vec_func>(&f)) {
                if (auto* vector_y = std::get_if<vec_d>(&y)) {
                    vec_d k_1 = (*vector_f)(t, *vector_y);
                    vec_d k_2 = (*vector_f)(t + h/2, *vector_y + k_1*h/2);
                    vec_d k_3 = (*vector_f)(t + h/2, *vector_y + k_2*h/2);
                    vec_d k_4 = (*vector_f)(t +h, *vector_y + k_3*h);
                    y_next = *vector_y + h/6*(k_1 + 2*k_2 + 2*k_3 + k_4);
                } else {
                    throw std::runtime_error("Mismatched vector function with scalar input");
                }
            } 

            t += h;
            y = y_next;
            result.push_back(y);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error in RK4Solver::Solve: ") + e.what());
    }
    return result;
}

} // ScientficToolbox::ODE