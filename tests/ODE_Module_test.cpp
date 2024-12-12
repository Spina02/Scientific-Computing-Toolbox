#include "../include/ODE_Module/ODETester.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace ScientificToolbox::ODE;

int main() {
    ODETester tester;
    bool all_passed = true;

    // Test expression parser
    all_passed &= tester.run_parser_tests();
    
    // Test Forward Euler Solver
    all_passed &= tester.run_ode_tests();

    if (all_passed) {
        std::cout << std::endl << std::endl << "All tests passed!" << std::endl;
    } else {
        std::cerr << std::endl << std::endl << "Some tests failed!" << std::endl;
    }

    std::cout << std::endl << "------------ Analysis Section ------------" << std::endl;

    Func f {parseExpression("y")}; // Dummy function
    double solution = std::exp(1); // Dummy solution
    double t0 = 0;
    double tf = 1;
    double h = 0.01;
    double y0 = 1;

    for (auto solver_type : solver_types) {
        // Create solver with unique_ptr
        auto solver_ptr = factories[solver_type](f, t0, y0, tf, h);
        auto [sol, time] = solve_and_measure_execution_time(*solver_ptr);
        double error = compute_error(sol.get_result(), solution);
        double order = compute_order_of_convergence(solver_type);
        
        std::cout << "Solver: " << solver_type << std::endl;
        std::cout << "  Error: " << error << std::endl;
        std::cout << "  Order of Convergence: " << order << std::endl;
        std::cout << "  Elapsed Time: " << time << std::endl;
    } // solver_ptr automatically deleted here

    return 0;
}