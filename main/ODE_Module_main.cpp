
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "../include/ODE_Module/ODE_Module.hpp"
#include "../include/Utilities.hpp"

using namespace std::filesystem;
using namespace ScientificToolbox::ODE;

int main(int argc, char** argv) {
    try {
        // Default values
        path project_dir = std::filesystem::current_path();
        path inputFile = project_dir / path("data") / path("ode_examples.csv");
        path outputFolder = project_dir / path("output");

        std::cout << "Enter input filename from data folder (press Enter for default):\n";
        std::string userInput;
        std::getline(std::cin, userInput);
        if (!userInput.empty()) {
            inputFile = project_dir / "data" / userInput;
        }

        std::cout << "ODE Module Demo\n" << std::endl;

        // You can load the data from a CSV file using the Importer class
        std::vector<ODETestCase> test_cases = load_tests_from_csv(inputFile);

        /*
         * NOTE: 
         *   The ODE solvers return an ODESolution object:
         *       you can modify the number of steps to print changing
         *       the value of ODESolution.steps_to_print [default = 10]) 
         *       Use ODESolution.steps_to_print = ODESolution.size to print all steps
         *       Use the get_result() method to access the final value
         */

        std::cout << std::endl;
        for (const auto& test : test_cases) {
            std::cout << std::endl << test << std::endl;
            double t0 = test.t0;
            double tf = test.tf;
            double h = test.h;
            var_vec y0 = test.y0; 

            std::cout << std::endl << "  Solving ODE using different methods:" << std::endl << std::endl;
            
            // reset files
            std::ofstream(outputFolder / "ODe_Module" / "euler_solution.csv", std::ios::trunc);
            std::ofstream(outputFolder / "ODe_Module" / "midpoint_solution.csv", std::ios::trunc);
            std::ofstream(outputFolder / "ODe_Module" / "rk4_solution.csv", std::ios::trunc);

            // Solve using Forward Euler
            auto euler = ForwardEulerSolver(test.expr, y0, t0, tf, h);
            auto euler_solution = euler.solve();
            std::cout << "  Forward Euler Method:\t\t";
            // use the result field to access the final value
            std::cout << euler_solution.get_result() << std::endl;
            save_to_csv(outputFolder / "euler_solution.csv", euler_solution, true);

            // Solve using Explicit Midpoint
            auto midpoint = ExplicitMidpointSolver(test.expr, y0, t0, tf, h);
            auto midpoint_solution = midpoint.solve();
            std::cout << "  Explicit Midpoint Method:\t";
            // use the result field to access the final value
            std::cout << midpoint_solution.get_result() << std::endl;
            save_to_csv(outputFolder / "midpoint_solution.csv", midpoint_solution, true);

            // Solve using RK4
            auto rk4 = RK4Solver(test.expr, y0, t0, tf, h);
            auto rk4_solution = rk4.solve();
            std::cout << "  Runge-Kutta 4th Order Method:\t";
            // use the result field to access the final value
            std::cout << rk4_solution.get_result() << std::endl;
            save_to_csv(outputFolder / "rk4_solution.csv", rk4_solution, true);
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}