
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
#include <filesystem>

using namespace ScientificToolbox::Interpolation;

InterpolationTester::InterpolationTester() {}

/** @method run_tests method
 * @brief Method to run all interpolation tests
 * 
 * This method runs all the interpolation tests, including linear, Lagrange, Newton, and cubic spline interpolation.
 * It generates test data for linear, quadratic, and cubic functions, and tests the interpolation methods on this data.
 * 
 */
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

/** @method import_data method
 * @brief Method to test the import of data
 * 
 * This method tests the import of data from CSV files for linear, quadratic, and cubic functions.
 * It reads the data from the CSV files and checks for any errors in the data.
 * 
 * @return bool True if the data is imported successfully, false otherwise
 * 
 */
bool InterpolationTester::import_data() {
    std::string project_dir = std::filesystem::current_path().parent_path();
    std::string filename_1 = project_dir + "/data/linear_data.csv";
    std::string filename_2 = project_dir + "/data/quadratic_data.csv";
    std::string filename_3 = project_dir + "/data/cubic_data.csv";

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

/** @method check_data method
 * @brief Method to check the imported data
 * 
 * This method checks the imported data for uniqueness and sorted order of x values.
 * It ensures that there are no duplicate points in the data and that the x values are sorted.
 * 
 * @return bool True if the data is checked successfully, false otherwise
 * 
 */
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

/** @method LinearInterpolationTest method
 * @brief Method to test linear interpolation
 * 
 * This method tests the linear interpolation method on the generated linear data.
 * It interpolates the y values for the x values x1, x2, and x3 and checks the results.
 * 
 * @return bool True if the linear interpolation test passes, false otherwise
 * 
 */
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

/** @method LagrangeInterpolationTest method
 * @brief Method to test Lagrange interpolation
 * 
 * This method tests the Lagrange interpolation method on the generated quadratic data.
 * It interpolates the y values for the x values x1, x2, and x3 and checks the results.
 * 
 * @return bool True if the Lagrange interpolation test passes, false otherwise
 * 
 */
bool InterpolationTester::LagrangeInterpolationTest() {

    // Test Lagrange interpolation
    Lagrange<double> lagrange(quadratic_data);

    // Test interpolation at known points
    double y1 = lagrange(x1);
    if (y1 - 10.75 > epsilon) {
        std::cerr << "Error: Lagrange interpolation failed at x = 1.5. Predicted value:" << y1 << std::endl;
        return false;
    }

    double y2 = lagrange(x2);
    if (y2 - 44.75 > epsilon) {
        std::cerr << "Error: Lagrange interpolation failed at x = 3.5." << std::endl;
        return false;
    }

    double y3 = lagrange(x3);
    if (y3 - 102.75 > epsilon) {
        std::cerr << "Error: Lagrange interpolation failed at x = 5.5." << std::endl;
        return false;
    }

    return true;
}

/** @method NewtonInterpolationTest method
 * @brief Method to test Newton interpolation
 * 
 * This method tests the Newton interpolation method on the generated quadratic data.
 * It interpolates the y values for the x values x1, x2, and x3 and checks the results.
 * 
 * @return bool True if the Newton interpolation test passes, false otherwise
 * 
 */
bool InterpolationTester::NewtonInterpolationTest() {

    // Test Newton interpolation
    Newton<double> newton(quadratic_data);

    // Test interpolation at known points
    double y1 = newton(x1);
    if (y1 - 10.75 > epsilon) {
        std::cerr << "Error: Newton interpolation failed at x = 1.5." << std::endl;
        return false;
    }

    double y2 = newton(x2);
    if (y2 - 44.75 > epsilon) {
        std::cerr << "Error: Newton interpolation failed at x = 3.5." << std::endl;
        return false;
    }

    double y3 = newton(x3);
    if (y3 - 102.75 > epsilon) {
        std::cerr << "Error: Newton interpolation failed at x = 5.5." << std::endl;
        return false;
    }

    return true;
}

/** @method CubicSplineInterpolationTest method
 * @brief Method to test cubic spline interpolation
 * 
 * This method tests the cubic spline interpolation method on the generated cubic data.
 * It interpolates the y values for the x values x1, x2, and x3 and checks the results.
 * 
 * @return bool True if the cubic spline interpolation test passes, false otherwise
 * 
 */
bool InterpolationTester::CubicSplineInterpolationTest() {

    // Test Cubic Spline interpolation
    SplineInterpolation<double> spline(cubic_data);

    // Test interpolation at known points
    double y1 = spline(x1);
    if (y1 - 25.623 > epsilon) {
        std::cerr << "Error: Cubic Spline interpolation failed at x = 1.5. Predicted value: " << y1 << std::endl;
        return false;
    }

    double y2 = spline(x2);
    if (y2 - 216.353 > epsilon) {
        std::cerr << "Error: Cubic Spline interpolation failed at x = 3.5. Predicted value: " << y2 << std::endl;
        return false;
    }

    double y3 = spline(x3);
    if (y3 - 768.315 > epsilon) {
        std::cerr << "Error: Cubic Spline interpolation failed at x = 5.5. Predicted value: "<< y3 << std::endl;
        return false;
    }

    return true;
}

