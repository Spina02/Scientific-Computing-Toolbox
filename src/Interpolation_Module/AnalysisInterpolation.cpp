#include "../../include/Interpolation_Module/utilities_interpolation.hpp"
#include "../../include/Interpolation_Module/AnalysisInterpolation.hpp"
#include "../../include/Interpolation_Module/Interpolation.hpp"
#include "../../include/Interpolation_Module/LinearInterpolation.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <cmath>

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

void AnalysisInterpolation::AccuracyTest() {
    std::cout << "Starting Accuracy Test..." << std::endl;
    std::vector<double> y_true;

    // Generating y_true values
    for (const auto& x : random_x) {
        y_true.push_back(generator_function(x));
    }

    // Obtaining x values from the set of points
    std::vector<double> x_values;
    for (const auto& point : points) {
        x_values.push_back(point.get_x());
    }

    // Linear Interpolation
    LinearInterpolation<double> linear(points);
    std::vector<double> y_pred_linear;
    for (const auto& x : x_values) {
        y_pred_linear.push_back(linear(x));
    }

    // Displaying mae
    std::cout << "Mean Absolute Error (Linear Interpolation): " << mae(y_true, y_pred_linear) << std::endl;

    
}
