#include "../include/Interpolation_Module.hpp"

#include <iostream>
#include <vector>
#include <utility> // For std::pair
#include <random> // For random number generation

// random number generator function guaranteeing unique values
double random_generator_unique(std::vector<double> &values, std::mt19937 &gen, std::uniform_real_distribution<double> &dist) {
    double random_value = dist(gen);
    while (std::find(values.begin(), values.end(), random_value) != values.end()) {
        random_value = dist(gen);
    }
    return random_value;
}

// random number generator function
double random_generator(std::mt19937 &gen, std::uniform_real_distribution<double> &dist) {
    return dist(gen);
}


int main() {
    std::vector<point> data = {{0.0, 0.0}, {1.0, 1.0}, {2.0, 4.0}, {3.0, 9.0}};
    LinearInterpolation linear(data);

    try {
        std::cout << "f(1.5) = " << linear(1.5) << "\n";
        std::cout << "f(2.5) = " << linear(2.5) << "\n";
        std::cout << "f(4.0) = " << linear(4.0) << "\n"; // Should throw an exception
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    return 0;
}