#include "../../include/Interpolation_Module/utilities_interpolation.hpp"
#include "../../include/Interpolation_Module/AnalysisInterpolation.hpp"
#include "../../include/Interpolation_Module/Interpolation.hpp"
#include "../../include/Interpolation_Module/LinearInterpolation.hpp"
#include "../../include/Interpolation_Module/Lagrange.hpp"
#include "../../include/Interpolation_Module/Newton.hpp"
#include "../../include/Interpolation_Module/Cubic_Spline_Interpolation.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <chrono>

using namespace ScientificToolbox::Interpolation;

// Define the constructor properly here
AnalysisInterpolation::AnalysisInterpolation(std::set<point<double>> points, 
                                             std::function<double(double)> generator_function, 
                                             std::vector<double> random_x) 
    : points(std::move(points)), generator_function(std::move(generator_function)), random_x(std::move(random_x)) {}

AnalysisInterpolation::~AnalysisInterpolation() {}

double AnalysisInterpolation::mae(std::vector<double> y_true, std::vector<double> y_pred) {
    if (y_true.size() != y_pred.size()) {
        throw std::invalid_argument("Mean Absolute Error: input vectors must have the same size.");
    }

    double sum = 0.0;
    for (size_t i = 0; i < y_true.size(); ++i) {
        sum += std::abs(y_true[i] - y_pred[i]);
    }

    return sum / y_true.size();
}

void AnalysisInterpolation::AccuracyAnalysis() {
    bool exception_thrown = false;
    try {    
        std::cout << "\nStarting Accuracy Analysis..." << std::endl;
        std::vector<double> y_true;

        // Generating y_true values
        for (const auto& x : random_x) {
            y_true.push_back(generator_function(x));
        }

        // Linear Interpolation
        LinearInterpolation<double> linear(points);
        std::vector<double> y_pred_linear;
        for (const auto& x : random_x) {
            y_pred_linear.push_back(linear(x));
        }

        // Lagrange Interpolation
        Lagrange<double> lagrange(points);
        std::vector<double> y_pred_lagrange;
        for (const auto& x : random_x) {
            y_pred_lagrange.push_back(lagrange(x));
        }

        // Newton Interpolation
        Newton<double> newton(points);
        std::vector<double> y_pred_newton;
        for (const auto& x : random_x) {
            y_pred_newton.push_back(newton(x));
        }

        // Cubic Spline Interpolation
        SplineInterpolation<double> cubic_spline(points);
        std::vector<double> y_pred_cubic_spline;
        for (const auto& x : random_x) {
            y_pred_cubic_spline.push_back(cubic_spline(x));
        }

        // Displaying mae
        std::cout << "Mean Absolute Error (Linear Interpolation): " << mae(y_true, y_pred_linear) << std::endl;
        std::cout << "Mean Absolute Error (Lagrange Interpolation): " << mae(y_true, y_pred_lagrange) << std::endl;
        std::cout << "Mean Absolute Error (Newton Interpolation): " << mae(y_true, y_pred_newton) << std::endl;
        std::cout << "Mean Absolute Error (Cubic Spline Interpolation): " << mae(y_true, y_pred_cubic_spline) << std::endl;
    } catch (const std::exception& e) {
        exception_thrown = true;
    }

    if (exception_thrown) {
        std::cerr << "Error occurred during Accuracy Analysis." << std::endl;
    } else {
        std::cout << "Accuracy Analysis completed successfully." << std::endl;
    }

    
}

void AnalysisInterpolation::EfficiencyAnalysis() {
    std::cout << "Starting Efficiency Analysis..." << std::endl;
    bool exception_thrown = false;
    try{
        // Linear Interpolation
        auto start = std::chrono::high_resolution_clock::now();
        LinearInterpolation<double> linear(points);
        linear.interpolate(random_x[0]);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Linear Interpolation: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;

        // Lagrange Interpolation
        start = std::chrono::high_resolution_clock::now();
        Lagrange<double> lagrange(points);
        lagrange.interpolate(random_x[0]);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Lagrange Interpolation: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;

        // Newton Interpolation
        start = std::chrono::high_resolution_clock::now();
        Newton<double> newton(points);
        newton.interpolate(random_x[0]);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Newton Interpolation: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;

        // Cubic Spline Interpolation
        start = std::chrono::high_resolution_clock::now();
        SplineInterpolation<double> cubic_spline(points);
        cubic_spline.interpolate(random_x[0]);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Cubic Spline Interpolation: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;
    } catch (const std::exception& e) {
        exception_thrown = true;
    }

    if (exception_thrown) {
        std::cerr << "Error occurred during Efficiency Analysis." << std::endl;
    } else {
        std::cout << "Efficiency Analysis completed successfully." << std::endl;
    }
}

void AnalysisInterpolation::OrderConvergenceAnalysis() {
    std::cout << "Starting Order Convergence Analysis..." << std::endl;

    // Known function for Analysising
    auto Analysis_function = [](double x) { return std::sin(x); };

    // Number of Analysis points
    size_t num_Analysis_points = 100;
    std::vector<double> Analysis_points(num_Analysis_points);
    std::vector<double> Analysis_values(num_Analysis_points);

    // Generate Analysis points and values
    double x_min = 0.0;
    double x_max = 2.0 * M_PI;
    double step = (x_max - x_min) / (num_Analysis_points - 1);
    for (size_t i = 0; i < num_Analysis_points; ++i) {
        Analysis_points[i] = x_min + i * step;
        Analysis_values[i] = Analysis_function(Analysis_points[i]);
    }

    // Number of interpolation points
    std::vector<size_t> num_points = {10, 20, 40, 80, 160};

    // Perform the Analysis for each number of interpolation points
    for (size_t n : num_points) {
        std::set<point<double>> points;
        double h = (x_max - x_min) / (n - 1);
        for (size_t i = 0; i < n; ++i) {
            double x = x_min + i * h;
            points.insert(point<double>(x, Analysis_function(x)));
        }

        // Linear Interpolation
        LinearInterpolation<double> linear(points);
        double linear_error = 0.0;
        for (size_t i = 0; i < num_Analysis_points; ++i) {
            double error = std::abs(linear.interpolate(Analysis_points[i]) - Analysis_values[i]);
            linear_error += error * error;
        }
        linear_error = std::sqrt(linear_error / num_Analysis_points);
        std::cout << "Linear Interpolation (n = " << n << "): Error = " << linear_error << std::endl;

        // Lagrange Interpolation
        Lagrange<double> lagrange(points);
        double lagrange_error = 0.0;
        for (size_t i = 0; i < num_Analysis_points; ++i) {
            double error = std::abs(lagrange.interpolate(Analysis_points[i]) - Analysis_values[i]);
            lagrange_error += error * error;
        }
        lagrange_error = std::sqrt(lagrange_error / num_Analysis_points);
        std::cout << "Lagrange Interpolation (n = " << n << "): Error = " << lagrange_error << std::endl;

        // Newton Interpolation
        Newton<double> newton(points);
        double newton_error = 0.0;
        for (size_t i = 0; i < num_Analysis_points; ++i) {
            double error = std::abs(newton.interpolate(Analysis_points[i]) - Analysis_values[i]);
            newton_error += error * error;
        }
        newton_error = std::sqrt(newton_error / num_Analysis_points);
        std::cout << "Newton Interpolation (n = " << n << "): Error = " << newton_error << std::endl;

        // Cubic Spline Interpolation
        SplineInterpolation<double> spline(points);
        double spline_error = 0.0;
        for (size_t i = 0; i < num_Analysis_points; ++i) {
            double error = std::abs(spline.interpolate(Analysis_points[i]) - Analysis_values[i]);
            spline_error += error * error;
        }
        spline_error = std::sqrt(spline_error / num_Analysis_points);
        std::cout << "Cubic Spline Interpolation (n = " << n << "): Error = " << spline_error << std::endl;
    }

    std::cout << "Order Convergence Analysis completed successfully." << std::endl;
}