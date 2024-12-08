#include "../include/Interpolation_Module/small_classes.hpp"
#include "../include/Interpolation_Module/Interpolation.hpp"
#include "../include/Interpolation_Module/LinearInterpolation.hpp"
#include "../include/Interpolation_Module/PolynomialInterpolation.hpp"
#include "../include/Utilities/ImportCSV.hpp"
#include "../include/Utilities/ImportData.hpp"
#include "../include/Utilities/Lagrange_coefficients.hpp"
#include "../include/Interpolation_Module/Newton_coefficients.hpp"

#include <iostream>
#include <vector>
#include <utility> // For std::pair
#include <random>  // For random number generation
#include <algorithm> // For std::find_if
#include <cmath>   // For isnan
#include <set>     // For unique random number generation

using namespace ScientificToolbox;


int main() {

    // Import data from CSV file 
    std::string filename = "../../data/points_R2.csv";
    ImportCSV importer;
    std::set<point<double>> points = importer.read_points_from_csv<double>(filename);

    // Print the points in the set
    for (const auto& p : points) {
        std::cout << "(" << p.get_x() << ", " << p.get_y() << ")" << std::endl;
    }


    try {

        // Initialize interpolation with a set of points
        LinearInterpolation<double> linear(points);

        // Polynomial interpolation
        Lagrange<double> poly(points);
        Newton<double> newton(points);
    
        // Interpolate
        std::cout << "Insert x value for interpolation: ";
        double x;
        std::cin >> x;

        // Linear interpolation result
        std::cout << "Lagrange Interpolated value: " << linear(x) << "\n";

        // Polynomial interpolation result
        std::cout << "Polynomial Interpolated value: " << poly(x) << "\n";

        // Print the polynomial in the form y = a0 + a1*x + a2*x^2 + ...    
        std::vector<double> coefficients = poly.polynomial();
        std::cout << "Lagrange Polynomial: ";
        for (size_t i = 0; i < coefficients.size(); ++i) {
            std::cout << coefficients[i] << "*x^" << i;
            if (i < coefficients.size() - 1) {
                std::cout << " + ";
            }
        }
        std::cout << "\n";

        // Polynomial Interpolation using Newton's method
        std::cout << "Newton Interpolated value: " << newton(x) << "\n";

        // Print the polynomial in the form y = a0 + a1*x + a2*x^2 + ...
        std::vector<double> newton_coefficients = newton.newton_coefficients();
        std::cout << "Newton Polynomial: ";
        for (size_t i = 0; i < newton_coefficients.size(); ++i) {
            std::cout << newton_coefficients[i] << "*x^" << i;
            if (i < newton_coefficients.size() - 1) {
                std::cout << " + ";
            }
        }
        std::cout << "\n";



    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
