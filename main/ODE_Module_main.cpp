
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "../include/ODE_Module.hpp"
#include "../include/Utilities/ImportCSV.hpp"

using namespace ScientificToolbox::ODE;

int main(int argc, char** argv) {

    std::string project_dir = std::filesystem::current_path().parent_path();

    try {
        // Default values
        std::string inputFile = project_dir + "/data/ode_examples.csv";
        std::string outputFile = project_dir + "/output/ODE_Module_output.csv";

        // Parse command line arguments
        if (argc > 1) {
            inputFile = project_dir + "/data/" + std::string(argv[1]);;
        }
        if (argc > 2) {
            outputFile = project_dir + "/data/" + std::string(argv[2]);;
        }

        std::cout << "ODE Module Demo\n" << std::endl;

        // You can load the data from a CSV file using the ImportCSV class
        load_tests_from_csv(inputFile);

        /*
         * NOTE: 
         *   The ODE solvers return an ODESolution object:
         *       you can modify the number of steps to print changing
         *       the value of ODESolution.steps [default = 10]) 
         *       Use ODESolution.steps = ODESolution.size to print all steps
         *       Use the get_result() method to access the final value
         */

        std::cout << std::endl;
        for (const auto& test : cases) {
            std::cout << std::endl << test << std::endl;
            Func f{parseExpression(test.expr)};
            double t0 = test.t0;
            double tf = test.tf;
            double h = test.h;
            var_vec y0 = test.y0; 

            std::cout << std::endl << "  Solving ODE using different methods:" << std::endl << std::endl;
            
            // Solve using Forward Euler
            auto euler = ForwardEulerSolver(f, t0, y0, tf, h);
            auto euler_solution = euler.Solve();
            std::cout << "  Forward Euler Method:\t\t";
            // use the result field to access the final value
            std::cout << euler_solution.get_result() << std::endl;

            // Solve using Explicit Midpoint
            auto midpoint = ExplicitMidpointSolver(f, t0, y0, tf, h);
            auto midpoint_solution = midpoint.Solve();
            std::cout << "  Explicit Midpoint Method:\t";
            // use the result field to access the final value
            std::cout << midpoint_solution.get_result() << std::endl;

            // Solve using RK4
            auto rk4 = RK4Solver(f, t0, y0, tf, h);
            auto rk4_solution = rk4.Solve();
            std::cout << "  Runge-Kutta 4th Order Method:\t";
            // use the result field to access the final value
            std::cout << rk4_solution.get_result() << std::endl;
        }

        // You can also define the test cases manually and parse them from strings
        std::cout << std::endl << "Manual Test Cases" << std::endl;
        // Example 1: Scalar ODE (dy/dt = y, y(0) = 1)
        std::cout << std::endl << "Example 1: Scalar ODE\n";
        var_expr scalar_expr = "y";  // dy/dt = y
        Func scalar_f{parseExpression(scalar_expr)};
        double t0 = 0.0;
        double tf = 1.0;
        double h = 0.001;
        var_vec y0 = 1.0;  // Initial condition y(0) = 1

        // Print problem description
        std::cout << "  Scalar ODE:" << std::endl;
        std::cout << "    dy/dt           :\t" << scalar_expr << std::endl;
        std::cout << "    Time interval   :\t[" << t0 << ", " << tf << "]" << std::endl;
        std::cout << "    Step size       :\t" << h << std::endl;
        std::cout << "    y(0)            :\t" << y0 << std::endl;

        // Solve using Forward Euler
        std::cout << " Forward Euler Method" << std::endl << std::endl;
        auto euler = ForwardEulerSolver(scalar_f, t0, y0, tf, h);
        auto euler_solution = euler.Solve(); // solve without measuring execution time
        //auto euler_solution = solve_and_measure_execution_time(euler); // solve and measure execution time
        // Print the trajectory using 2 steps
        euler_solution.steps = 2;
        std::cout << euler_solution << std::endl;

        // Solve using Explicit Midpoint
        std::cout << " Explicit Midpoint Method" << std::endl << std::endl;
        auto midpoint = ExplicitMidpointSolver(scalar_f, t0, y0, tf, h);
        auto midpoint_solution = midpoint.Solve(); // solve without measuring execution time
        //auto midpoint_solution = solve_and_measure_execution_time(midpoint); // solve and measure execution time
        // Print the trajectory using 4 steps
        midpoint_solution.steps = 4;
        std::cout << midpoint_solution << std::endl;

        // Solve using RK4
        std::cout << " Runge-Kutta 4th Order Method" << std::endl << std::endl;
        auto rk4 = RK4Solver(scalar_f, t0, y0, tf, h);
        auto rk4_solution = rk4.Solve(); // solve without measuring execution time
        //auto rk4_solution = solve_and_measure_execution_time(rk4); // solve and measure execution time
        // Print the trajectory using 6 steps
        rk4_solution.steps = 6;
        std::cout << rk4_solution << std::endl;

        // Save results to CSV
        save_on_CSV("../../output/ODE_Module/scalar_ode_results_FE.csv", euler_solution);
        std::cout << "results saved on output/ODE_Module/scalar_ode_results_FE.csv" << std::endl;
        save_on_CSV("../../output/ODE_Module/scalar_ode_results_EM.csv", midpoint_solution);
        std::cout << "results saved on output/ODE_Module/scalar_ode_results_EM.csv" << std::endl;
        save_on_CSV("../../output/ODE_Module/scalar_ode_results_RK4.csv", rk4_solution);
        std::cout << "results saved on output/ODE_Module/scalar_ode_results_RK4.csv" << std::endl;

        // Example 2: Vector ODE (predator-prey model)
        std::cout << std::endl << "Example 2: Vector ODE (Predator-Prey Model)" << std::endl << std::endl;
        vec_s vector_expr = {"0.1 * y1 - 0.02 * y1 * y2", "-0.3 * y2 + 0.01 * y1 * y2"};
        Func vector_f{parseExpression(vector_expr)};
        
        // Initial conditions: [prey, predator]
        vec_d vec_y0(2);
        vec_y0 << 50.0, 20.0;
        var_vec vector_y0 = vec_y0;

        //Print problem description
        std::cout << "  Predator-Prey Model:" << std::endl;
        std::cout << "    dy1/dt           :\t" << vector_expr[0] << std::endl;
        std::cout << "    dy2/dt           :\t" << vector_expr[1] << std::endl;
        std::cout << "    Time interval    :\t[" << t0 << ", " << tf << "]" << std::endl;
        std::cout << "    Time step        :\t" << h << std::endl;
        std::cout << "    y(0)             :\t" << vector_y0 << std::endl;

        // Solve using Forward Euler
        auto vector_euler = ForwardEulerSolver(vector_f, t0, vector_y0, tf, h);
        auto vector_euler_solution= vector_euler.Solve();
        vector_euler_solution.steps = 5;
        std::cout << vector_euler_solution << std::endl;

        // Solve using Explicit Midpoint
        auto vector_midpoint = ExplicitMidpointSolver(vector_f, t0, vector_y0, tf, h);
        auto vector_midpoint_solution= vector_midpoint.Solve();
        vector_midpoint_solution.steps = 5;
        std::cout << vector_midpoint_solution << std::endl;

        // Solve using RK4
        auto vector_rk4 = RK4Solver(vector_f, t0, vector_y0, tf, h);
        auto vector_solution= vector_rk4.Solve();
        vector_solution.steps = 5;
        std::cout << vector_solution << std::endl;

        save_on_CSV("../../output/ODE_Module/vector_ode_results_FE.csv", vector_euler_solution);
        std::cout << "results saved on output/ODE_Module/vector_ode_results_FE.csv" << std::endl;
        save_on_CSV("../../output/ODE_Module/vector_ode_results_EM.csv", vector_midpoint_solution);
        std::cout << "results saved on output/ODE_Module/vector_ode_results_EM.csv" << std::endl;
        save_on_CSV("../../output/ODE_Module/vector_ode_results_RK4.csv", vector_solution);
        std::cout << "results saved on output/ODE_Module/vector_ode_results_RK4.csv" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}