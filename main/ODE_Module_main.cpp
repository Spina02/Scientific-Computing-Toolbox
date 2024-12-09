#include "../include/ODE_Module/ODETester.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace ScientificToolbox::ODE;

int main() {
    ODETester tester;
    bool all_passed = true;

    // Test expression parser
    all_passed &= tester.test_expression_parser();
    
    // Test Forward Euler Solver
    all_passed &= tester.test_Solvers();

    if (all_passed) {
        std::cout << std::endl << std::endl << "All tests passed!" << std::endl;
    } else {
        std::cerr << std::endl << std::endl << "Some tests failed!" << std::endl;
    }

    return 0;
}