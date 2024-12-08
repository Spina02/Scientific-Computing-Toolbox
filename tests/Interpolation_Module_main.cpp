#include "../include/Interpolation_Module/small_classes.hpp"
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

using namespace ScientificToolbox;

int main() {

    // Import data from CSV file 
    std::string filename = "../../data/points_R2.csv";
    ImportCSV importer;
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

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
