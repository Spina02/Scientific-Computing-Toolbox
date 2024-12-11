
#include "../include/Interpolation_Module/InterpolationTester.hpp"
#include <iostream>

using namespace ScientificToolbox::Interpolation;

int main() {
    InterpolationTester tester;
    bool all_passed = true;

    all_passed &= tester.run_tests();

    if (all_passed) {
        std::cout << std::endl << std::endl << "All tests passed!" << std::endl;
    } else {
        std::cerr << std::endl << std::endl << "Some tests failed!" << std::endl;
    }

    return 0;
}