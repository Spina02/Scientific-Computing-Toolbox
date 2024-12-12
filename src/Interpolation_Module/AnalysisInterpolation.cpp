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

/** @Constructor
 * @brief Constructor for the AnalysisInterpolation class
 * 
 * This constructor initializes the analysis of the interpolation method using the data provided.
 * 
 */
AnalysisInterpolation::AnalysisInterpolation(std::set<point<double>> points, 
                                             std::function<double(double)> generator_function, 
                                             std::vector<double> random_x) 
    : points(std::move(points)), generator_function(std::move(generator_function)), random_x(std::move(random_x)) {}

AnalysisInterpolation::~AnalysisInterpolation() {}

/** @method for calculating the Mean Absolute Error (MAE)
 * @brief Method for calculating the Mean Absolute Error (MAE)
 * 
 * This method calculates the Mean Absolute Error (MAE) between two vectors of values.
 * 
 * @param y_true True values
 * @param y_pred Predicted values
 * @return Mean Absolute Error (MAE)
 * 
 */
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

/** @ method Accuracy Analysis function
 * @brief Accuracy Analysis function for the AnalysisInterpolation class
 * 
 * This function performs an accuracy analysis of the interpolation method.
 * 
 */
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

/** @method Efficiency Analysis function
 * @brief Efficiency Analysis function for the AnalysisInterpolation class
 * 
 * This function performs an efficiency analysis of the interpolation method.
 * 
 */
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

/** @method Order Convergence Analysis function
 * @brief Order Convergence Analysis function for the AnalysisInterpolation class
 * 
 * This function performs an order convergence analysis of the interpolation method.
 * 
 */
void AnalysisInterpolation::OrderConvergenceAnalysis() {
    std::cout << "Starting Order Convergence Analysis..." << std::endl;

    std::vector<double> x_1 {1.5, 2.5};
    std::vector<double> x_2 {1.25, 1.5, 1.75, 2.25, 2.5, 2.75};
    std::vector<double> x_3 {1.125, 1.25, 1.375, 1.5, 1.625, 1.75, 1.875, 2.125, 2.25, 2.375, 2.5, 2.625, 2.75, 2.875};

    // generating y_true
    std::vector<double> y_true_1, y_true_2, y_true_3;
    for (const auto& x : x_1) {
        y_true_1.push_back(generator_function(x));
    }
    for (const auto& x : x_2) {
        y_true_2.push_back(generator_function(x));
    }
    for (const auto& x : x_3) {
        y_true_3.push_back(generator_function(x));
    }

    // Linear Interpolation
    LinearInterpolation<double> linear(points);
    std::vector<double> y_pred_linear_1, y_pred_linear_2, y_pred_linear_3;
    for (const auto& x : x_1) {
        y_pred_linear_1.push_back(linear(x));
    }
    for (const auto& x : x_2) {
        y_pred_linear_2.push_back(linear(x));
    }
    for (const auto& x : x_3) {
        y_pred_linear_3.push_back(linear(x));
    }

    // Error calculation
    double linear_mae_1 = mae(y_true_1, y_pred_linear_1);
    double linear_mae_2 = mae(y_true_2, y_pred_linear_2);
    double linear_mae_3 = mae(y_true_3, y_pred_linear_3);

    // Order Convergence Analysis
    double linear_order_1 = std::log2(linear_mae_2 / linear_mae_1) / std::log2(0.25/0.5);
    double linear_order_2 = std::log2(linear_mae_3 / linear_mae_2) / std::log2(0.125/0.25);

    // Lagrange Interpolation
    Lagrange<double> lagrange(points);
    std::vector<double> y_pred_lagrange_1, y_pred_lagrange_2, y_pred_lagrange_3;
    for (const auto& x : x_1) {
        y_pred_lagrange_1.push_back(lagrange(x));
    }
    for (const auto& x : x_2) {
        y_pred_lagrange_2.push_back(lagrange(x));
    }
    for (const auto& x : x_3) {
        y_pred_lagrange_3.push_back(lagrange(x));
    }

    // Error calculation
    double lagrange_mae_1 = mae(y_true_1, y_pred_lagrange_1);
    double lagrange_mae_2 = mae(y_true_2, y_pred_lagrange_2);
    double lagrange_mae_3 = mae(y_true_3, y_pred_lagrange_3);

    // Order Convergence Analysis
    double lagrange_order_1 = std::log2(lagrange_mae_2 / lagrange_mae_1) / std::log2(0.25/0.5);
    double lagrange_order_2 = std::log2(lagrange_mae_3 / lagrange_mae_2) / std::log2(0.125/0.25);

    // Newton Interpolation
    Newton<double> newton(points);
    std::vector<double> y_pred_newton_1, y_pred_newton_2, y_pred_newton_3;
    for (const auto& x : x_1) {
        y_pred_newton_1.push_back(newton(x));
    }
    for (const auto& x : x_2) {
        y_pred_newton_2.push_back(newton(x));
    }
    for (const auto& x : x_3) {
        y_pred_newton_3.push_back(newton(x));
    }

    // Error calculation
    double newton_mae_1 = mae(y_true_1, y_pred_newton_1);
    double newton_mae_2 = mae(y_true_2, y_pred_newton_2);
    double newton_mae_3 = mae(y_true_3, y_pred_newton_3);

    // Order Convergence Analysis
    double newton_order_1 = std::log2(newton_mae_2 / newton_mae_1) / std::log2(0.25/0.5);
    double newton_order_2 = std::log2(newton_mae_3 / newton_mae_2) / std::log2(0.125/0.25);

    // Cubic Spline Interpolation
    SplineInterpolation<double> cubic_spline(points);
    std::vector<double> y_pred_cubic_spline_1, y_pred_cubic_spline_2, y_pred_cubic_spline_3;
    for (const auto& x : x_1) {
        y_pred_cubic_spline_1.push_back(cubic_spline(x));
    }
    for (const auto& x : x_2) {
        y_pred_cubic_spline_2.push_back(cubic_spline(x));
    }
    for (const auto& x : x_3) {
        y_pred_cubic_spline_3.push_back(cubic_spline(x));
    }

    // Error calculation
    double cubic_spline_mae_1 = mae(y_true_1, y_pred_cubic_spline_1);
    double cubic_spline_mae_2 = mae(y_true_2, y_pred_cubic_spline_2);
    double cubic_spline_mae_3 = mae(y_true_3, y_pred_cubic_spline_3);

    // Order Convergence Analysis
    double cubic_spline_order_1 = std::log2(cubic_spline_mae_2 / cubic_spline_mae_1) / std::log2(0.25/0.5);
    double cubic_spline_order_2 = std::log2(cubic_spline_mae_3 / cubic_spline_mae_2) / std::log2(0.125/0.25);

    // Displaying errors and orders
    std::cout << "Linear Interpolation errors: " << linear_mae_1 << " " << linear_mae_2 << " " << linear_mae_3 << std::endl;
    std::cout << "Linear Interpolation Order: " << linear_order_1 << " " << linear_order_2 << std::endl;

    std::cout << "Lagrange Interpolation errors: " << lagrange_mae_1 << " " << lagrange_mae_2 << " " << lagrange_mae_3 << std::endl;
    std::cout << "Lagrange Interpolation Order: " << lagrange_order_1 << " " << lagrange_order_2 << std::endl;

    std::cout << "Newton Interpolation errors: " << newton_mae_1 << " " << newton_mae_2 << " " << newton_mae_3 << std::endl;
    std::cout << "Newton Interpolation Order: " << newton_order_1 << " " << newton_order_2 << std::endl;

    std::cout << "Cubic Spline Interpolation errors: " << cubic_spline_mae_1 << " " << cubic_spline_mae_2 << " " << cubic_spline_mae_3 << std::endl;
    std::cout << "Cubic Spline Interpolation Order: " << cubic_spline_order_1 << " " << cubic_spline_order_2 << std::endl;

    std::cout << "Order Convergence Analysis completed successfully." << std::endl;
}
