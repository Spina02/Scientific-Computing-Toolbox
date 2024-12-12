
#include "../include/Interpolation_Module/InterpolationTester.hpp"
#include "../include/Interpolation_Module/AnalysisInterpolation.hpp"
#include "../include/Interpolation_Module/utilities_interpolation.hpp"
#include <iostream>
#include <vector>
#include <random>

using namespace ScientificToolbox::Interpolation;

/** @function random_value_generator function
 * @brief Function to generate random values
 * 
 * This function generates random values between 1.0 and 10.0.
 * 
 */
double random_value_generator() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(1.0, 10.0);
    return dis(gen);
}

/** main function
 * @brief Main function for the Interpolation module test
 * 
 * This function tests the Interpolation module by running the tests and analyzing the interpolation methods
 * 
 * For analysis, it uses 10 x values and a generator function y=x^6 to generate the y values, and then generates
 * 100 random x values to test the interpolation methods.
 * 
 * @return 0
 *  
 */
int main() {
    std::cout << "\n TEST SECTION" << std::endl;

    InterpolationTester tester;

    tester.run_tests();

    std::cout << "\nANALYSIS SECTION" << std::endl;
    
    std::vector<double> x_values = {1.0, 2.0, 3.0};

    // printing x values
    std::cout << "X values: ";
    for (const auto& x : x_values) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    // Function x^2
    std::function<double(double)> generator_function = [](double x) { return std::pow(x, 2); };
    std::cout << "Generator function: y=x^2" << std::endl;

    // Generating y_true values
    std::vector<double> y_true;
    for (const auto& x : x_values) {
        y_true.push_back(generator_function(x));
    }

    // printing y_true values
    std::cout << "Y values: ";
    for (const auto& y : y_true) {
        std::cout << y << " ";
    }

    // Generating points
    std::set<point<double>> points;
    for (size_t i = 0; i < x_values.size(); ++i) {
        points.insert(point<double>(x_values[i], y_true[i]));
    }

    std::vector<double> random_x{1.25, 1.75, 2.25, 2.75};

    AnalysisInterpolation analysis(points, generator_function, random_x);
    analysis.AccuracyAnalysis();
    std::cout << std::endl;
    analysis.EfficiencyAnalysis();
    std::cout << std::endl;
    analysis.OrderConvergenceAnalysis();


    return 0;
}