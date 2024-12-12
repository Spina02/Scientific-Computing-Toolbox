#ifndef INTERPOLATIONTESTER_HPP
#define INTERPOLATIONTESTER_HPP

#include <set>
#include "utilities_interpolation.hpp"

/** @namespace ScientificToolbox::Interpolation
 * @brief Interpolation module providing basic interpolation functions
 * 
 * This namespace contains template classes for common interpolation methods:
 * - Linear interpolation
 * - Polynomial interpolation
 * - Spline interpolation
 */

namespace ScientificToolbox::Interpolation{
    
    /** @class InterpolationTester class
     * @brief Class for testing interpolation methods
     * 
     * This class provides methods for testing the interpolation methods in the Interpolation module.
     * It generates test data for linear, quadratic, and cubic functions, and tests the interpolation
     * methods on this data.
     * 
     */
    class InterpolationTester{
    public:
        InterpolationTester();
        ~InterpolationTester() = default;

        void run_tests();
    
    private:
        bool import_data();
        bool check_data();
        bool LinearInterpolationTest();
        bool LagrangeInterpolationTest();
        bool NewtonInterpolationTest();
        bool CubicSplineInterpolationTest();
        
        std::set<point<double>> linear_data; // generated with linear function y = 2x + 1
        std::vector<double> x_linear_data;
        std::set<point<double>> quadratic_data; // generated with quadratic function y = 3x^2 + 2x + 1
        std::vector<double> x_quadratic_data;
        std::set<point<double>> cubic_data; // generated with cubic function y = 4x^3 + 3x^2 + 2x + 1
        std::vector<double> x_cubic_data;

        const double x1 = 1.5;
        const double x2 = 3.5;
        const double x3 = 5.5;

        double epsilon = 1e-3;
        bool DEBUG = false;
    };
}


#endif