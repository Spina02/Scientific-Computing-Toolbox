#include "../include/Interpolation_Module/small_classes.hpp"
#include "../include/Interpolation_Module/Interpolation.hpp"
#include "../include/Interpolation_Module/LinearInterpolation.hpp"
#include "../include/Interpolation_Module/PolynomialInterpolation.hpp"

#include <iostream>
#include <vector>
#include <utility> // For std::pair
#include <random>  // For random number generation
#include <algorithm> // For std::find_if
#include <cmath>   // For isnan
#include <set>     // For unique random number generation

using namespace ScientificToolbox;

// Random number generator
template <typename T>
std::set<point<T>> random_numbers_generator(size_t n, T lower_bound, T upper_bound, bool unique = true) {
    std::set<point<T>> random_numbers; // Change to set to ensure uniqueness

    std::random_device rd; // seed
    std::mt19937 gen(rd()); // random number generator
    std::uniform_real_distribution<T> dis(lower_bound, upper_bound); // distribution of random numbers

    while (random_numbers.size() < n) {
        T x = dis(gen);
        T y = dis(gen);
        random_numbers.emplace(x, y);  // Insert directly into set
    }
    
    return random_numbers;
}

int main() {

    std::cout << "Enter the number of data points: ";
    size_t n;
    std::cin >> n;
    std::cout << "Enter the lower bound: ";
    double lower_bound;
    std::cin >> lower_bound;
    std::cout << "Enter the upper bound: ";
    double upper_bound;
    std::cin >> upper_bound;

    // Generate random data
    auto data = random_numbers_generator<double>(n, lower_bound, upper_bound);

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
