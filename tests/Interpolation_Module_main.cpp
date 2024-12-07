#include "../include/Interpolation_Module/small_classes.hpp"
#include "../include/Interpolation_Module/Interpolation.hpp"
#include "../include/Interpolation_Module/LinearInterpolation.hpp"
#include "../include/Interpolation_Module/PolynomialInterpolation.hpp"
#include "../include/Statistics_Module/ImportData.hpp"
#include "../include/Statistics_Module/ImportCSV.hpp"

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
    try {
        importer.import(filename);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    std::set<point<double>> data = importer.toPointSet();

    // Print the generated data for verification
    std::cout << "Generated data points:\n";
    for (const auto& p : data) {
        std::cout << "x: " << p.get_x() << ", y: " << p.get_y() << "\n";
    }

    // Since set is already sorted by x, no need to sort
    std::cout << "Min x: " << data.begin()->get_x() << ", Max x: " << std::prev(data.end())->get_x() << "\n";

    try {
        // Initialize interpolation with a set of points
        LinearInterpolation<double> linear(data);
    
        // Interpolate
        std::cout << "Insert x value for interpolation: ";
        double x;
        std::cin >> x;
        std::cout << "Interpolated value: " << linear(x) << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
