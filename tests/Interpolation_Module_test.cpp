
#include "../include/Interpolation_Module/DataTester.hpp"
#include "../include/Interpolation_Module/AnalysisInterpolation.hpp"
#include "../include/Interpolation_Module/utilities_interpolation.hpp"
#include <iostream>
#include <vector>
#include <random>

using namespace ScientificToolbox::Interpolation;

double random_value_generator() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(1.0, 10.0);
    return dis(gen);
}

int main() {
    InterpolationTester tester;
    
    std::vector<double> x_values = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

    // Function x^6
    std::function<double(double)> generator_function = [](double x) { return std::pow(x, 6); };

    // Generating y_true values
    std::vector<double> y_true;
    for (const auto& x : x_values) {
        y_true.push_back(generator_function(x));
    }

    // Generating points
    std::set<point<double>> points;
    for (size_t i = 0; i < x_values.size(); ++i) {
        points.insert(point<double>(x_values[i], y_true[i]));
    }

    // Random unique x values
    std::vector<double> random_x;
    for (size_t i = 0; i < 100; ++i) {
        double random_x_value = random_value_generator();
        while (std::find(random_x.begin(), random_x.end(), random_x_value) != random_x.end()) {
            random_x_value = random_value_generator();
        }
        random_x.push_back(random_x_value);
    }

    AnalysisInterpolation analysis(points, generator_function, random_x);
    analysis.AccuracyAnalysis();
    std::cout << std::endl;
    analysis.EfficiencyAnalysis();
    std::cout << std::endl;
    analysis.OrderConvergenceAnalysis();


    return 0;
}