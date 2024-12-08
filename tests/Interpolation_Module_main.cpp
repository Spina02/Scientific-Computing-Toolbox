#include "../include/Interpolation_Module/utilities_interpolation.hpp"
#include "../include/Interpolation_Module/Interpolation.hpp"
#include "../include/Interpolation_Module/LinearInterpolation.hpp"
#include "../include/Interpolation_Module/PolynomialInterpolation.hpp"
#include "../include/Interpolation_Module/Lagrange.hpp"
#include "../include/Interpolation_Module/Newton.hpp"
#include "../include/Utilities/ImportCSV.hpp"
#include "../include/Utilities/ImportData.hpp"

#include <iostream>
#include <vector>
#include <utility> // For std::pair
#include <random>  // For random number generation
#include <algorithm> // For std::find_if
#include <cmath>   // For isnan
#include <set>     // For unique random number generation

/**
 * namespace ScientificToolbox::Interpolation
 * @brief Interpolation module providing basic interpolation functions
 * 
 * This namespace contains template classes for common interpolation methods:
 * - Linear interpolation
 * - Polynomial interpolation
 * - Spline interpolation
 */

/** importer
 * @brief Import data from a CSV file
 * 
 * This class provides functionality to import data from a CSV file.
 * It reads the file line by line and parses the data into a data structure.
 * 
 * The class is an abstract base class that defines the import method.
 * Derived classes can implement the import method for specific data types.
 * 
 * @see ImportCSV
 * 
 */

/** 
 * @file Interpolation_Module_main.cpp
 * @brief Main program for interpolation module
 * 
 * This program demonstrates the use of the interpolation module.
 * It reads data points from a CSV file and performs linear, Lagrange, and Newton interpolation.
 * 
 * Required includes:
 * - <iostream>          - For standard I/O operations
 * - <set>               - For std::set data structure
 * - <vector>            - For dynamic arrays
 * - "ImportCSV.hpp"     - For CSV file importing
 * - "LinearInterpolation.hpp" - For linear interpolation
 * - "Lagrange.hpp"      - For Lagrange interpolation
 * - "Newton.hpp"        - For Newton interpolation
 * 
 * @return 0 on success, prints error message to stderr on failure
 *  
 * @throws std::runtime_error If an error occurs during interpolation
 * 
 */


using namespace ScientificToolbox;

int main() {

    // Import data from CSV file 
    std::string filename = "../../data/points_R2.csv";
    ScientificToolbox::ImportCSV importer;
    std::set<point<double>> points = importer.read_points_from_csv<double>(filename);

    // Print the points in the set
    std::cout << "Data Points:" << std::endl;
    for (const auto& p : points) {
        std::cout << "(" << p.get_x() << ", " << p.get_y() << ")" << std::endl;
    }

    try {
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
        // printing the coefficients in the form y = a + bx + cx^2 + ...
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
        // printing the coefficients in the form y = a + bx + cx^2 + ...
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

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}