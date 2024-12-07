#include "../include/Interpolation_Module/small_classes.hpp"
#include "../include/Interpolation_Module/Interpolation.hpp"
#include "../include/Interpolation_Module/LinearInterpolation.hpp"
#include "../include/Interpolation_Module/PolynomialInterpolation.hpp"
#include "../include/Utilities/ImportCSV.hpp"
#include "../include/Utilities/ImportData.hpp"
#include "../include/Utilities/Lagrange_coefficients.hpp"

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
    
        // Interpolate
        std::cout << "Insert x value for interpolation: ";
        double x;
        std::cin >> x;

        // Linear interpolation result
        std::cout << "Linear Interpolated value: " << linear(x) << "\n";

        // Polynomial interpolation result
        std::cout << "Polynomial Interpolated value: " << poly(x) << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
