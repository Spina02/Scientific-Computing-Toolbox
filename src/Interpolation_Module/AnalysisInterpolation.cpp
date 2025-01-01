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
AnalysisInterpolation::AnalysisInterpolation() {}

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
double AnalysisInterpolation::AccuracyAnalysis(std::set<point<double>> true_points, std::set<point<double>> sparse_points, std::string interpolation_method) {
    bool exception_thrown = false;
    try {    
        std::vector<double> x_true, y_true, x_sparse, y_sparse;

        // Generating true data
        for (const auto& point : true_points) {
            x_true.push_back(point.get_x());
            y_true.push_back(point.get_y());
        }

        // Generating sparse data
        for (const auto& point : sparse_points) {
            x_sparse.push_back(point.get_x());
            y_sparse.push_back(point.get_y());
        }

        if(interpolation_method == "linear") {
            std::vector<double> y_pred_linear;
            LinearInterpolation<double> linear(sparse_points);
            for (const auto& x : x_true) {
                y_pred_linear.push_back(linear(x));
            }
            return mae(y_true, y_pred_linear);
        } else if (interpolation_method == "lagrange") {
            std::vector<double> y_pred_lagrange;
            Lagrange<double> lagrange(sparse_points);
            for (const auto& x : x_true) {
                y_pred_lagrange.push_back(lagrange(x));
            }
            return mae(y_true, y_pred_lagrange);
        } else if (interpolation_method == "newton") {
            std::vector<double> y_pred_newton;
            Newton<double> newton(sparse_points);
            for (const auto& x : x_true) {
                y_pred_newton.push_back(newton(x));
            }
            return mae(y_true, y_pred_newton);
        } else if (interpolation_method == "cubic_spline") {
            std::vector<double> y_pred_cubic_spline;
            SplineInterpolation<double> cubic_spline(sparse_points);
            for (const auto& x : x_true) {
                y_pred_cubic_spline.push_back(cubic_spline(x));
            }
            return mae(y_true, y_pred_cubic_spline);
        } else {
            throw std::invalid_argument("Invalid interpolation method.");
        }

    } catch (const std::exception& e) {
        exception_thrown = true;
    }

    if (exception_thrown) {
        std::cerr << "Error occurred during Accuracy Analysis." << std::endl;
    } else {
        std::cout << "Accuracy Analysis completed successfully." << std::endl;
    }

    // Default return for error handling or invalid cases
    return std::numeric_limits<double>::quiet_NaN();
}

/** @method Efficiency Analysis function
 * @brief Efficiency Analysis function for the AnalysisInterpolation class
 * 
 * This function performs an efficiency analysis of the interpolation method.
 * 
 */
void AnalysisInterpolation::EfficiencyAnalysis(std::set<point<double>> true_points, std::set<point<double>> sparse_points, std::string interpolation_method) {

    bool exception_thrown = false;
    try{
        std::vector<double> x_true, y_true, x_sparse, y_sparse;

        // Generating true data
        for (const auto& point : true_points) {
            x_true.push_back(point.get_x());
            y_true.push_back(point.get_y());
        }

        // Generating sparse data
        for (const auto& point : sparse_points) {
            x_sparse.push_back(point.get_x());
            y_sparse.push_back(point.get_y());
        }

        if (interpolation_method == "linear") {
            // Linear Interpolation
            auto start = std::chrono::high_resolution_clock::now();
            LinearInterpolation<double> linear(true_points);
            linear.interpolate(x_sparse[0]);
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "Linear Interpolation: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;
        } else if (interpolation_method == "lagrange") {
            // Lagrange Interpolation
            auto start = std::chrono::high_resolution_clock::now();
            Lagrange<double> lagrange(true_points);
            lagrange.interpolate(x_sparse[0]);
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "Lagrange Interpolation: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;
        } else if (interpolation_method == "newton") {
            // Newton Interpolation
            auto start = std::chrono::high_resolution_clock::now();
            Newton<double> newton(true_points);
            newton.interpolate(x_sparse[0]);
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "Newton Interpolation: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;
        } else if (interpolation_method == "cubic_spline") {
            // Cubic Spline Interpolation
            auto start = std::chrono::high_resolution_clock::now();
            SplineInterpolation<double> cubic_spline(true_points);
            cubic_spline.interpolate(x_sparse[0]);
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "Cubic Spline Interpolation: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;
        } else {
            throw std::invalid_argument("Invalid interpolation method.");
        }
    } catch (const std::exception& e) {
        exception_thrown = true;
    }

    if (exception_thrown) {
        std::cerr << "Error occurred during Efficiency Analysis." << std::endl;
    } 
}

/** @method Order Convergence Analysis function
 * @brief Order Convergence Analysis function for the AnalysisInterpolation class
 * 
 * This function performs an order convergence analysis of the interpolation method.
 * 
 */
double AnalysisInterpolation::OrderConvergenceAnalysis(std::set<point<double>> true_points, std::set<point<double>> sparse_points_1, std::set<point<double>> sparse_points_2, std::string interpolation_method) {

    if (interpolation_method == "linear") {
        // Linear Interpolation
        double mae_1 = this->AccuracyAnalysis(true_points, sparse_points_1, "linear");
        double mae_2 = this->AccuracyAnalysis(true_points, sparse_points_2, "linear");
        return std::log2(mae_1 / mae_2);
    } else if (interpolation_method == "lagrange") {
        // Lagrange Interpolation
        double mae_1 = this->AccuracyAnalysis(true_points, sparse_points_1, "lagrange");
        double mae_2 = this->AccuracyAnalysis(true_points, sparse_points_2, "lagrange");
        return std::log2(mae_1 / mae_2);
    } else if (interpolation_method == "newton") {
        // Newton Interpolation
        double mae_1 = this->AccuracyAnalysis(true_points, sparse_points_1, "newton");
        double mae_2 = this->AccuracyAnalysis(true_points, sparse_points_2, "newton");
        return std::log2(mae_1 / mae_2);
    } else if (interpolation_method == "cubic_spline") {
        // Cubic Spline Interpolation
        double mae_1 = this->AccuracyAnalysis(true_points, sparse_points_1, "cubic_spline");
        double mae_2 = this->AccuracyAnalysis(true_points, sparse_points_2, "cubic_spline");
        return std::log2(mae_1 / mae_2);
    } else {
        throw std::invalid_argument("Invalid interpolation method.");
    }
}
