#include "../include/Interpolation_Module/utilities_interpolation.hpp"
#include "../include/Interpolation_Module/Interpolation.hpp"
#include "../include/Interpolation_Module/LinearInterpolation.hpp"
#include "../include/Interpolation_Module/PolynomialInterpolation.hpp"
#include "../include/Interpolation_Module/Cubic_Spline_Interpolation.hpp"
#include "../include/Interpolation_Module/Lagrange.hpp"
#include "../include/Interpolation_Module/Newton.hpp"
#include "../include/Utilities/ImportCSV.hpp"
#include "../include/Utilities/ImportData.hpp"

#include <iostream>
#include <vector>
#include <utility>   // For std::pair
#include <random>    // For random number generation
#include <algorithm> // For std::find_if
#include <cmath>     // For isnan
#include <set>       // For unique random number generation
#include <limits>    // For infinity

using namespace ScientificToolbox::Interpolation;

int main() {



    // Import data from CSV file 
    std::string filename = "../../data/uniform_points_R2.csv";
    ScientificToolbox::ImportCSV importer;
    
    try {
        std::set<point<double>> points = importer.read_points_from_csv<double>(filename);

        // Find the minimum and maximum x values
        interval<double> min_max = minmax_search(points);
        std::cout << "Minimum x value: " << min_max.get_lower_bound() << std::endl;
        std::cout << "Maximum x value: " << min_max.get_upper_bound() << std::endl;

        // Linear Interpolation Test
        LinearInterpolation<double> linear(points);
        std::cout << "\nLinear Interpolation:" << std::endl;
        std::cout << "Insert a value for x: ";
        double x;
        std::cin >> x;
        std::cout << "Interpolated value at x = " << x << ": " << linear(x) << std::endl;

        // Lagrange Interpolation Test
        Lagrange<double> lagrange(points);
        std::cout << "\nLagrange Interpolation:" << std::endl;
        std::cout << "Insert a value for x: ";
        std::cin >> x;
        std::cout << "Interpolated value at x = " << x << ": " << lagrange(x) << std::endl;
        
        std::vector<double> lagrange_coefficients = lagrange.compute_lagrange_coefficients();
        std::cout << "Lagrange Coefficients: ";
        for (size_t i = 0; i < lagrange_coefficients.size(); ++i) {
            std::cout << lagrange_coefficients[i];
            if (i > 0) {
                std::cout << "x^" << i;
            }
            if (i < lagrange_coefficients.size() - 1) {
                std::cout << " + ";
            }
        }
        std::cout << std::endl;

        // Newton Interpolation Test
        Newton<double> newton(points);
        std::cout << "\nNewton Interpolation:" << std::endl;
        std::cout << "Insert a value for x: ";
        std::cin >> x;
        std::cout << "Interpolated value at x = " << x << ": " << newton(x) << std::endl;
        
        std::vector<double> newton_coefficients = newton.newton_coefficients();
        std::cout << "Newton Coefficients: ";
        for (size_t i = 0; i < newton_coefficients.size(); ++i) {
            std::cout << newton_coefficients[i];
            if (i > 0) {
                std::cout << "x^" << i;
            }
            if (i < newton_coefficients.size() - 1) {
                std::cout << " + ";
            }
        }
        std::cout << std::endl;

        // Cubic Spline Interpolation Test
        CubicSplineInterpolation<double> cubic_spline(points);
        std::cout << "\nCubic Spline Interpolation:" << std::endl;
        std::cout << "Insert a value for x: ";
        std::cin >> x;
        std::cout << "Interpolated value at x = " << x << ": " << cubic_spline(x) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error occurred during interpolation: " << e.what() << "\n";
    }

    return 0;
}
