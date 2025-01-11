#include <iostream>
#include <vector>
#include <utility>   // For std::pair
#include <random>    // For random number generation
#include <algorithm> // For std::find_if
#include <cmath>     // For isnan
#include <set>       // For unique random number generation
#include <limits>    // For infinity
#include <filesystem> // For file existence check

#include "../include/Interpolation_Module.hpp"

using namespace ScientificToolbox::Interpolation;

int main(int argc, char** argv) {

    // Project directory
    std::string project_dir = std::filesystem::current_path();//.parent_path();

    // Default values
    std::string filename = project_dir + "/data/random_data.csv";
    double x = 1.5;

    std::cout << "argc: " << argc << std::endl;

    for (int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }

    if (argc > 1) {
        filename = project_dir + "/data/" + std::string(argv[1]);
    }
    else if (argc > 2) {
        x = std::stod(argv[2]);
    }
    else if (argc > 3) {
        std::cout << "\nToo many arguments. Using only the first two arguments." << std::endl;
    }
    else {
        std::cout << "\nNo args entered. Using " << filename << " and " << x << " as value to interpolate..."<< std::endl;
    }
    
    ScientificToolbox::Importer importer;
    
    try {
        std::set<point<double>> points = read_points_from_csv<double>(filename);

        // Linear Interpolation Test
        LinearInterpolation<double> linear(points);
        std::cout << "\nLinear Interpolation:" << std::endl;
        std::cout << "Interpolated value at x = " << x << ": " << linear(x) << std::endl;

        // Lagrange Interpolation Test
        Lagrange<double> lagrange(points);
        std::cout << "\nLagrange Interpolation:" << std::endl;
        std::cout << "Interpolated value at x = " << x << ": " << lagrange(x) << std::endl;

        // Newton Interpolation Test
        Newton<double> newton(points);
        std::cout << "\nNewton Interpolation:" << std::endl;
        std::cout << "Interpolated value at x = " << x << ": " << newton(x) << std::endl;

        // Cubic Spline Interpolation Test
        SplineInterpolation<double> spline(points);
        std::cout << "\nCubic Spline Interpolation:" << std::endl;
        std::cout << "Interpolated value at x = " << x << ": " << spline(x) << std::endl;
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error occurred during interpolation: " << e.what() << "\n";
    }

    return 0;
}
