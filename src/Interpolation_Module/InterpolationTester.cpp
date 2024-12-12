
#include "../../include/Interpolation_Module/InterpolationTester.hpp"
#include "../../include/Interpolation_Module/Interpolation.hpp"
#include "../../include/Interpolation_Module/Interpolation.hpp"
#include "../../include/Interpolation_Module/LinearInterpolation.hpp"
#include "../../include/Interpolation_Module/Cubic_Spline_Interpolation.hpp"
#include "../../include/Interpolation_Module/Lagrange.hpp"
#include "../../include/Interpolation_Module/Newton.hpp"
#include "../../include/Utilities/ImportCSV.hpp"

#include <iostream>
#include <vector>

using namespace ScientificToolbox::Interpolation;

InterpolationTester::InterpolationTester() {}

void InterpolationTester::run_tests() {
    std::cout << "Running interpolation tests..." << std::endl;

    std::cout << "Testing import data..." << std::endl;
    if (import_data()){
        std::cout << "Data imported successfully." << std::endl;
    } else {
        std::cerr << "Error importing data." << std::endl;
        return;
    }

    std::cout << "Checking data..." << std::endl;
    if (check_data()){
        std::cout << "Data checked successfully." << std::endl;
    } else {
        std::cerr << "Error checking data." << std::endl;
        return;
    }

    std::cout << "Testing linear interpolation..." << std::endl;
    if (LinearInterpolationTest()){
        std::cout << "Linear interpolation test passed." << std::endl;
    } else {
        std::cerr << "Linear interpolation test failed." << std::endl;
    }

    std::cout << "Testing Lagrange interpolation..." << std::endl;
    if (LagrangeInterpolationTest()){
        std::cout << "Lagrange interpolation test passed." << std::endl;
    } else {
        std::cerr << "Lagrange interpolation test failed." << std::endl;
    }

    std::cout << "Testing Newton interpolation..." << std::endl;
    if (NewtonInterpolationTest()){
        std::cout << "Newton interpolation test passed." << std::endl;
    } else {
        std::cerr << "Newton interpolation test failed." << std::endl;
    }

    std::cout << "Testing Cubic Spline interpolation..." << std::endl;
    if (CubicSplineInterpolationTest()){
        std::cout << "Cubic Spline interpolation test passed." << std::endl;
    } else {
        std::cerr << "Cubic Spline interpolation test failed." << std::endl;
    }

    
}

bool InterpolationTester::import_data() {
    std::string filename_1 = "../../data/linear_data.csv";
    std::string filename_2 = "../../data/quadratic_data.csv";
    std::string filename_3 = "../../data/cubic_data.csv";

    ScientificToolbox::ImportCSV importer;

    try {
        linear_data = importer.read_points_from_csv<double>(filename_1);
        quadratic_data = importer.read_points_from_csv<double>(filename_2);
        cubic_data = importer.read_points_from_csv<double>(filename_3);
        if (DEBUG) {
            std::cout << "Imported linear data points:" << std::endl;
            for (const auto& point : linear_data) {
                std::cout << "x: " << point.get_x() << ", y: " << point.get_y() << std::endl;
            }
            std::cout << "Imported quadratic data points:" << std::endl;
            for (const auto& point : quadratic_data) {
                std::cout << "x: " << point.get_x() << ", y: " << point.get_y() << std::endl;
            }
            std::cout << "Imported cubic data points:" << std::endl;
            for (const auto& point : cubic_data) {
                std::cout << "x: " << point.get_x() << ", y: " << point.get_y() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error importing data: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool InterpolationTester::check_data() {
    // Checking uniqueness of data points

    for (const auto& point : linear_data) {
        if (linear_data.count(point) > 1) {
            std::cerr << "Error: Duplicate data points in linear_data." << std::endl;
            return false;
        }
    }
    for (const auto& point : quadratic_data) {
        if (quadratic_data.count(point) > 1) {
            std::cerr << "Error: Duplicate data points in quadratic_data." << std::endl;
            return false;
        }
    }
    for (const auto& point : cubic_data) {
        if (cubic_data.count(point) > 1) {
            std::cerr << "Error: Duplicate data points in cubic_data." << std::endl;
            return false;
        }
    }

    // Checking uniqueness of x values of each data point

    for (const auto& point : linear_data) {
        x_linear_data.push_back(point.get_x());
    }
    for (const auto& point : quadratic_data) {
        x_quadratic_data.push_back(point.get_x());
    }
    for (const auto& point : cubic_data) {
        x_cubic_data.push_back(point.get_x());
    }

    if (DEBUG) {
        std::cout << "X values of linear_data:" << std::endl;
        for (const auto& x : x_linear_data) {
            std::cout << x << std::endl;
        }
        std::cout << "X values of quadratic_data:" << std::endl;
        for (const auto& x : x_quadratic_data) {
            std::cout << x << std::endl;
        }
        std::cout << "X values of cubic_data:" << std::endl;
        for (const auto& x : x_cubic_data) {
            std::cout << x << std::endl;
        }
    }

    for (size_t i = 0; i < x_linear_data.size(); ++i) {
        for (size_t j = i + 1; j < x_linear_data.size(); ++j) {
            if (x_linear_data[i] == x_linear_data[j]) {
                std::cerr << "Error: Duplicate x values in linear_data." << std::endl;
                return false;
            }
        }
    }
    for (size_t i = 0; i < x_quadratic_data.size(); ++i) {
        for (size_t j = i + 1; j < x_quadratic_data.size(); ++j) {
            if (x_quadratic_data[i] == x_quadratic_data[j]) {
                std::cerr << "Error: Duplicate x values in quadratic_data." << std::endl;
                return false;
            }
        }
    }
    for (size_t i = 0; i < x_cubic_data.size(); ++i) {
        for (size_t j = i + 1; j < x_cubic_data.size(); ++j) {
            if (x_cubic_data[i] == x_cubic_data[j]) {
                std::cerr << "Error: Duplicate x values in cubic_data." << std::endl;
                return false;
            }
        }
    }

    // Checking sorted order of x values of each data point

    for (size_t i = 0; i < x_linear_data.size() - 1; ++i) {
        if (x_linear_data[i] > x_linear_data[i + 1]) {
            std::cerr << "Error: Unsorted x values in linear_data." << std::endl;
            return false;
        }
    }
    for (size_t i = 0; i < x_quadratic_data.size() - 1; ++i) {
        if (x_quadratic_data[i] > x_quadratic_data[i + 1]) {
            std::cerr << "Error: Unsorted x values in quadratic_data." << std::endl;
            return false;
        }
    }
    for (size_t i = 0; i < x_cubic_data.size() - 1; ++i) {
        if (x_cubic_data[i] > x_cubic_data[i + 1]) {
            std::cerr << "Error: Unsorted x values in cubic_data." << std::endl;
            return false;
        }
    }

    return true;
}

bool InterpolationTester::LinearInterpolationTest() {
    LinearInterpolation<double> linear_interpolation(linear_data);

    // Test interpolation at known points
    double x1 = 1.5;
    double y1 = linear_interpolation(x1);
    if (y1 - 4.0 > epsilon) {
        std::cerr << "Error: Linear interpolation failed at x = 1.5. Predicted value:" << y1 << std::endl;
        return false;
    }

    double x2 = 3.5;
    double y2 = linear_interpolation(x2);
    if (y2 - 8.0 > epsilon) {
        std::cerr << "Error: Linear interpolation failed at x = 3.5. Predicted value:" << y2 << std::endl;
        return false;
    }

    double x3 = 5.5;
    double y3 = linear_interpolation(x3);
    if (y3 - 12.0 > epsilon) {
        std::cerr << "Error: Linear interpolation failed at x = 5.5. Predicted value:" << y3 << std::endl;
        return false;
    }

    return true;
}

bool InterpolationTester::LagrangeInterpolationTest() {

    // Test Lagrange interpolation
    Lagrange<double> lagrange_interpolation(quadratic_data);

    // Test interpolation at known points
    double y1 = lagrange_interpolation(x1);
    if (y1 - 6.75 > epsilon) {
        std::cerr << "Error: Lagrange interpolation failed at x = 1.5. Predicted value:" << y1 << std::endl;
        return false;
    }

    double y2 = lagrange_interpolation(x2);
    if (y2 - 30.25 > epsilon) {
        std::cerr << "Error: Lagrange interpolation failed at x = 3.5." << std::endl;
        return false;
    }

    double y3 = lagrange_interpolation(x3);
    if (y3 - 84.75 > epsilon) {
        std::cerr << "Error: Lagrange interpolation failed at x = 5.5." << std::endl;
        return false;
    }

    return true;
}

bool InterpolationTester::NewtonInterpolationTest() {

    // Test Newton interpolation
    Newton<double> newton_interpolation(quadratic_data);

    // Test interpolation at known points
    double y1 = newton_interpolation(x1);
    if (y1 - 6.75 > epsilon) {
        std::cerr << "Error: Newton interpolation failed at x = 1.5." << std::endl;
        return false;
    }

    double y2 = newton_interpolation(x2);
    if (y2 - 30.25 > epsilon) {
        std::cerr << "Error: Newton interpolation failed at x = 3.5." << std::endl;
        return false;
    }

    double y3 = newton_interpolation(x3);
    if (y3 - 84.75 > epsilon) {
        std::cerr << "Error: Newton interpolation failed at x = 5.5." << std::endl;
        return false;
    }

    return true;
}

bool InterpolationTester::CubicSplineInterpolationTest() {

    // Test Cubic Spline interpolation
    SplineInterpolation<double> cubic_spline_interpolation(cubic_data);

    // Test interpolation at known points
    double y1 = cubic_spline_interpolation(x1);
    if (y1 - 13.0 > epsilon) {
        std::cerr << "Error: Cubic Spline interpolation failed at x = 1.5." << std::endl;
        return false;
    }

    double y2 = cubic_spline_interpolation(x2);
    if (y2 - 57.0 > epsilon) {
        std::cerr << "Error: Cubic Spline interpolation failed at x = 3.5." << std::endl;
        return false;
    }

    double y3 = cubic_spline_interpolation(x3);
    if (y3 - 157.0 > epsilon) {
        std::cerr << "Error: Cubic Spline interpolation failed at x = 5.5." << std::endl;
        return false;
    }

    return true;
}

