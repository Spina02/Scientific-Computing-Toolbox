#ifndef ANALYSISINTERPOLATION_HPP
#define ANALYSISINTERPOLATION_HPP

#include "utilities_interpolation.hpp"

#include <vector>
#include <functional>
#include <set>

/**
 * namespace ScientificToolbox::Interpolation
 * @brief Interpolation module providing basic interpolation functions
 * 
 * This namespace contains template classes for common interpolation methods:
 * - Linear interpolation
 * - Polynomial interpolation
 * - Spline interpolation
 */

/** Constructor
 * @brief Constructor for the AnalysisInterpolation class
 * 
 * This constructor initializes the analysis of the interpolation method using the data provided.
 * 
 * @param points Set of points to interpolate
 * @param generator_function Function to generate the data
 * @param random_x Random x values to test the interpolation
 * 
 */

/** Destructor
 * @brief Destructor for the AnalysisInterpolation class
 * 
 * This destructor frees the memory allocated for the analysis of the interpolation method.
 * 
 */

/** Accuracy Analysis function
 * @brief Accuracy Analysis function for the AnalysisInterpolation class
 * 
 * This function performs an accuracy analysis of the interpolation method.
 * 
 */

/** Efficiency Analysis function
 * @brief Efficiency Analysis function for the AnalysisInterpolation class
 * 
 * This function performs an efficiency analysis of the interpolation method.
 * 
 */

/** Order Convergence Analysis function
 * @brief Order Convergence Analysis function for the AnalysisInterpolation class
 * 
 * This function performs an order convergence analysis of the interpolation method.
 * 
 */

namespace ScientificToolbox::Interpolation {
    class AnalysisInterpolation {
    public:
        AnalysisInterpolation(std::set<point<double>> points, std::function<double(double)> generator_function, std::vector<double> random_x);
        ~AnalysisInterpolation();

        void AccuracyAnalysis();
        void EfficiencyAnalysis();
        void OrderConvergenceAnalysis();

    private:
        std::set<point<double>> points;
        std::function<double(double)> generator_function;
        std::vector<double> random_x;

        double mae(std::vector<double> y_true, std::vector<double> y_pred);
    };
}

#endif
