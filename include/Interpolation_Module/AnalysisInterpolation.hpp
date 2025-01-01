#ifndef ANALYSISINTERPOLATION_HPP
#define ANALYSISINTERPOLATION_HPP

#include "utilities_interpolation.hpp"

#include <vector>
#include <functional>
#include <set>

/** @namespace ScientificToolbox::Interpolation
 * @brief Interpolation module providing basic interpolation functions
 * 
 * This namespace contains template classes for common interpolation methods:
 * - Linear interpolation
 * - Polynomial interpolation
 * - Spline interpolation
 */

namespace ScientificToolbox::Interpolation {
    
    /** @class AnalysisInterpolation
     * @brief Class for the analysis of the interpolation method
     * 
     * This class provides the analysis of the interpolation method using the data provided.
     * 
     * The class provides the following methods:
     * - Accuracy Analysis
     * - Efficiency Analysis
     * - Order Convergence Analysis 
     * 
     * The class also provides a Mean Absolute Error (MAE) function to calculate the error between the true and predicted values.
     * 
     * The class uses the following data:
     * - Set of points to interpolate
     * - Function to generate the data
     * - Random x values to test the interpolation
     * 
     */
    class AnalysisInterpolation {
    public:
        /** @method Constructor
         * @brief Constructor for the AnalysisInterpolation class
         * 
         * This constructor initializes the analysis of the interpolation method using the data provided.
         * 
         * @param points Set of points to interpolate
         * @param generator_function Function to generate the data
         * @param random_x Random x values to test the interpolation
         * 
         */
        AnalysisInterpolation();
        ~AnalysisInterpolation();

        double AccuracyAnalysis(std::set<point<double>> true_points, std::set<point<double>> sparse_points, std::string interpolation_method);
        void EfficiencyAnalysis(std::set<point<double>> true_points, std::set<point<double>> sparse_points, std::string interpolation_method);
        double OrderConvergenceAnalysis(std::set<point<double>> true_points, std::set<point<double>> sparse_points_1, std::set<point<double>> sparse_points_2, std::string interpolation_method);

    private:

        double mae(std::vector<double> y_true, std::vector<double> y_pred);
    };
}

#endif
