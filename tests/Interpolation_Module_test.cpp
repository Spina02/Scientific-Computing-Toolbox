
#include "../include/Interpolation_Module/InterpolationTester.hpp"
#include "../include/Interpolation_Module/AnalysisInterpolation.hpp"
#include "../include/Interpolation_Module/utilities_interpolation.hpp"
#include <iostream>
#include <vector>
#include <random>

using namespace ScientificToolbox::Interpolation;

std::vector<double> values_generator(double min, double max, size_t n, std::string equispaced = "true", std::string ordered = "true") {
    std::vector<double> values;
    if (equispaced == "true") {
        if (ordered == "true") {
            for (size_t i = 0; i < n; ++i) {
                values.push_back(min + i * (max - min) / (n - 1));
            }
        } else {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dis(min, max);
            for (size_t i = 0; i < n; ++i) {
                values.push_back(dis(gen));
            }
        }
    } else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(min, max);
        for (size_t i = 0; i < n; ++i) {
            values.push_back(dis(gen));
        }
    }
    return values;
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
    
    // Generating 20 values between 0 and 2*pi to feed the sine function
    std::vector<double> x_true = values_generator(0, 2 * M_PI, 20, "true", "true");

    // printing x values
    std::cout << "\nX true values: ";
    for (const auto& x : x_true) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    // Function to generate y values (y=sin(x))
    auto generator_function = [](double x) {
        return std::sin(x);
    };

    // Generating y_true values
    std::vector<double> y_true;
    for (const auto& x : x_true) {
        y_true.push_back(generator_function(x));
    }

    // printing y_true values
    std::cout << "\nY values: ";
    for (const auto& y : y_true) {
        std::cout << y << " ";
    }
    std::cout << std::endl;

    std::cout << "\nFunction used to generate y values: y = sin(x)" << std::endl;

    // Generating true points
    std::set<point<double>> true_points;
    for (size_t i = 0; i < x_true.size(); ++i) {
        true_points.insert(point<double>(x_true[i], y_true[i]));
    }

    // Generating x_sparse, 4 values equispaced between min(x_true) and max(x_true)
    std::vector<double> x_sparse = values_generator(x_true[0], x_true[x_true.size() - 1], 4, "true", "true");
    

    // printing x_sparse values
    std::cout << "\nX sparse values: ";
    for (const auto& x : x_sparse) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    // Generating y_sparse values
    std::vector<double> y_sparse;
    for (const auto& x : x_sparse) {
        y_sparse.push_back(generator_function(x));
    }

    // printing y_sparse values
    std::cout << "\nY sparse values: ";
    for (const auto& y : y_sparse) {
        std::cout << y << " ";
    }
    std::cout << std::endl;

    // Generating sparse points
    std::set<point<double>> sparse_points;
    for (size_t i = 0; i < x_sparse.size(); ++i) {
        sparse_points.insert(point<double>(x_sparse[i], y_sparse[i]));
    }

    // Analysis
    std::cout << "\nStarting Analysis..." << std::endl;
    AnalysisInterpolation analysis;
    
    // Accuracy Analysis
    std::cout << "\nAccuracy Analysis..." << std::endl;
    std::cout << "Mean Absolute Error (Linear Interpolation): " << analysis.AccuracyAnalysis(true_points, sparse_points, "linear") << std::endl;
    std::cout << "Mean Absolute Error (Lagrange Interpolation): " << analysis.AccuracyAnalysis(true_points, sparse_points, "lagrange") << std::endl;
    std::cout << "Mean Absolute Error (Newton Interpolation): " << analysis.AccuracyAnalysis(true_points, sparse_points, "newton") << std::endl;
    std::cout << "Mean Absolute Error (Cubic Spline Interpolation): " << analysis.AccuracyAnalysis(true_points, sparse_points, "cubic_spline") << std::endl;

    // Efficiency Analysis
    std::cout << "\nEfficiency Analysis..." << std::endl;
    analysis.EfficiencyAnalysis(true_points, sparse_points, "linear");
    analysis.EfficiencyAnalysis(true_points, sparse_points, "lagrange");
    analysis.EfficiencyAnalysis(true_points, sparse_points, "newton");
    analysis.EfficiencyAnalysis(true_points, sparse_points, "cubic_spline");

    // Order Convergence Analysis
    // Generating 8 values equispaced between min(x_true) and max(x_true)
    std::vector<double> x_sparse_2 = values_generator(x_true[0], x_true[x_true.size() - 1], 8, "true", "true");

    // Generating y_sparse_2 points
    std::set<point<double>> sparse_points_2;
    for (size_t i = 0; i < x_sparse_2.size(); ++i) {
        sparse_points_2.insert(point<double>(x_sparse_2[i], generator_function(x_sparse_2[i])));
    }

    std::cout << "\nOrder Convergence Analysis using 4 and 8 points..." << std::endl;
    std::cout << "Order Convergence Analysis (Linear Interpolation): " << analysis.OrderConvergenceAnalysis(true_points, sparse_points, sparse_points_2, "linear") << std::endl;
    std::cout << "Order Convergence Analysis (Lagrange Interpolation): " << analysis.OrderConvergenceAnalysis(true_points, sparse_points, sparse_points_2, "lagrange") << std::endl;
    std::cout << "Order Convergence Analysis (Newton Interpolation): " << analysis.OrderConvergenceAnalysis(true_points, sparse_points, sparse_points_2, "newton") << std::endl;
    std::cout << "Order Convergence Analysis (Cubic Spline Interpolation): " << analysis.OrderConvergenceAnalysis(true_points, sparse_points, sparse_points_2, "cubic_spline") << std::endl;




    return 0;
}