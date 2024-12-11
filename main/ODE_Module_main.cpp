
#include <iostream>
#include <vector>
#include <string>

#include "../include/ODE_Module.hpp"

using namespace ScientificToolbox::ODE;

int main() {
    try {
        std::cout << "ODE Module Demo\n" << std::endl;

        // Example 1: Scalar ODE (dy/dt = y, y(0) = 1)
        std::cout << "Example 1: Scalar ODE (dy/dt = y)\n";
        var_expr scalar_expr = "y";  // dy/dt = y
        Func scalar_f{parseExpression(scalar_expr)};
        double t0 = 0.0;
        double tf = 1.0;
        double h = 0.001;
        var_vec y0 = 1.0;  // Initial condition y(0) = 1

        // Print problem description
        std::cout << "Scalar ODE: " << scalar_expr << std::endl;
        std::cout << "Initial condition: y(0) = " << y0 << std::endl;
        std::cout << "Time interval: [" << t0 << ", " << tf << "]" << std::endl;

        // Solve using Forward Euler, Explicit Midpoint, and RK4
        auto euler = ForwardEulerSolver(scalar_f, t0, y0, tf, h);
        //auto euler_solution = euler.Solve(); // solve without measuring execution time
        auto euler_solution = solve_and_measure_execution_time(euler);

        // Solve using Explicit Midpoint
        auto midpoint = ExplicitMidpointSolver(scalar_f, t0, y0, tf, h);
        //auto midpoint_solution = midpoint.Solve(); // solve without measuring execution time
        auto midpoint_solution = solve_and_measure_execution_time(midpoint);

        // Solve using RK4
        auto rk4 = RK4Solver(scalar_f, t0, y0, tf, h);
        // auto rk4_solution = rk4.Solve(); // solve without measuring execution time
        auto rk4_solution = solve_and_measure_execution_time(rk4);

        // Print results
        // (you can modify the number of steps to print changing the value of ODESolution.steps [default = 10])
        std::cout << "Forward Euler Solution:" << std::endl;
        euler_solution.steps = 5; //
        std::cout << euler_solution << std::endl;
        std::cout << "Explicit Midpoint Solution:" << std::endl;
        std::cout << midpoint_solution << std::endl;
        std::cout << "RK4 Solution:" << std::endl;
        rk4_solution.steps = 15;
        std::cout << rk4_solution << std::endl;

        // Save results to CSV
        save_on_CSV("../../output/scalar_ode_results_FE.csv", euler_solution);
        std::cout << "results saved on output/ODE_Module/scalar_ode_results_FE.csv" << std::endl;
        save_on_CSV("../../output/scalar_ode_results_EM.csv", midpoint_solution);
        std::cout << "results saved on output/ODE_Module/scalar_ode_results_EM.csv" << std::endl;
        save_on_CSV("../../output/scalar_ode_results_RK4.csv", rk4_solution);
        std::cout << "results saved on output/ODE_Module/scalar_ode_results_RK4.csv" << std::endl;

        // Example 2: Vector ODE (predator-prey model)
        std::cout << "\nExample 2: Vector ODE (Predator-Prey Model)\n";
        vec_s vector_expr = {"0.1 * y1 - 0.02 * y1 * y2", "-0.3 * y2 + 0.01 * y1 * y2"};
        Func vector_f{parseExpression(vector_expr)};
        
        // Initial conditions: [prey, predator]
        vec_d vector_y0(2);
        vector_y0 << 50.0, 20.0;

        //Print problem description
        std::cout << "Predator-Prey Model:\n";
        std::cout << "dy1/dt = " << vector_expr[0] << std::endl;
        std::cout << "dy2/dt = " << vector_expr[1] << std::endl;
        std::cout << "Initial conditions: y1 = 50, y2 = 20\n";
        std::cout << "Time interval: [" << t0 << ", " << tf << "]" << std::endl;
        
        // Solve using Forward Euler
        auto vector_euler = ForwardEulerSolver(vector_f, t0, vector_y0, tf, h);
        auto vector_euler_result = vector_euler.Solve();
        save_on_CSV("../../output/vector_ode_results_FE.csv", vector_euler_result);
        std::cout << "results saved on output/ODE_Module/vector_ode_results_FE.csv" << std::endl;

        // Solve using Explicit Midpoint
        auto vector_midpoint = ExplicitMidpointSolver(vector_f, t0, vector_y0, tf, h);
        auto vector_midpoint_result = vector_midpoint.Solve();
        save_on_CSV("../../output/vector_ode_results_EM.csv", vector_midpoint_result);
        std::cout << "results saved on output/ODE_Module/vector_ode_results_EM.csv" << std::endl;

        // Solve using RK4
        auto vector_rk4 = RK4Solver(vector_f, t0, vector_y0, tf, h);
        auto vector_result = vector_rk4.Solve();
        save_on_CSV("../../output/vector_ode_results_RK4.csv", vector_result);
        std::cout << "results saved on output/ODE_Module/vector_ode_results_RK4.csv" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}