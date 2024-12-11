
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

    std::cout << "Testing cubic spline interpolation..." << std::endl;
    if (CubicSplineInterpolationTest()){
        std::cout << "Cubic spline interpolation test passed." << std::endl;
    } else {
        std::cerr << "Cubic spline interpolation test failed." << std::endl;
    }

    
}

bool InterpolationTester::import_data() {
    std::string filename_1 = "../../data/points_1_R2.csv";
    std::string filename_2 = "../../data/points_2_R2.csv";
    std::string filename_3 = "../../data/points_3_R2.csv";
    std::string filename_4 = "../../data/points_4_R2.csv";

    ScientificToolbox::ImportCSV importer;

    try {
        points_1 = importer.read_points_from_csv<double>(filename_1);
        points_2 = importer.read_points_from_csv<double>(filename_2);
        points_3 = importer.read_points_from_csv<double>(filename_3);
        points_4 = importer.read_points_from_csv<double>(filename_4);
    } catch (const std::exception& e) {
        std::cerr << "Error importing data: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool InterpolationTester::check_data() {
    // Checking uniqueness of data points

    for (const auto& point : points_1) {
        if (points_1.count(point) > 1) {
            std::cerr << "Error: Duplicate data points in points_1." << std::endl;
            return false;
        }
    }

    for (const auto& point : points_2) {
        if (points_2.count(point) > 1) {
            std::cerr << "Error: Duplicate data points in points_2." << std::endl;
            return false;
        }
    }

    for (const auto& point : points_3) {
        if (points_3.count(point) > 1) {
            std::cerr << "Error: Duplicate data points in points_3." << std::endl;
            return false;
        }
    }

    for (const auto& point : points_4) {
        if (points_4.count(point) > 1) {
            std::cerr << "Error: Duplicate data points in points_4." << std::endl;
            return false;
        }
    }

    // Checking uniqueness of x values of each data point

    for (const auto& point : points_1) {
        x_values_1.push_back(point.get_x());
    }

    for (const auto& point : points_2) {
        x_values_2.push_back(point.get_x());
    }

    for (const auto& point : points_3) {
        x_values_3.push_back(point.get_x());
    }

    for (const auto& point : points_4) {
        x_values_4.push_back(point.get_x());
    }

    for (size_t i = 0; i < x_values_1.size(); ++i) {
        for (size_t j = i + 1; j < x_values_1.size(); ++j) {
            if (x_values_1[i] == x_values_1[j]) {
                std::cerr << "Error: Duplicate x values in points_1." << std::endl;
                return false;
            }
        }
    }

    for (size_t i = 0; i < x_values_2.size(); ++i) {
        for (size_t j = i + 1; j < x_values_2.size(); ++j) {
            if (x_values_2[i] == x_values_2[j]) {
                std::cerr << "Error: Duplicate x values in points_2." << std::endl;
                return false;
            }
        }
    }

    for (size_t i = 0; i < x_values_3.size(); ++i) {
        for (size_t j = i + 1; j < x_values_3.size(); ++j) {
            if (x_values_3[i] == x_values_3[j]) {
                std::cerr << "Error: Duplicate x values in points_3." << std::endl;
                return false;
            }
        }
    }

    for (size_t i = 0; i < x_values_4.size(); ++i) {
        for (size_t j = i + 1; j < x_values_4.size(); ++j) {
            if (x_values_4[i] == x_values_4[j]) {
                std::cerr << "Error: Duplicate x values in points_4." << std::endl;
                return false;
            }
        }
    }

    // Checking sorted order of x values of each data point

    for (size_t i = 0; i < x_values_1.size() - 1; ++i) {
        if (x_values_1[i] > x_values_1[i + 1]) {
            std::cerr << "Error: Unsorted x values in points_1." << std::endl;
            return false;
        }
    }

    for (size_t i = 0; i < x_values_2.size() - 1; ++i) {
        if (x_values_2[i] > x_values_2[i + 1]) {
            std::cerr << "Error: Unsorted x values in points_2." << std::endl;
            return false;
        }
    }

    for (size_t i = 0; i < x_values_3.size() - 1; ++i) {
        if (x_values_3[i] > x_values_3[i + 1]) {
            std::cerr << "Error: Unsorted x values in points_3." << std::endl;
            return false;
        }
    }

    for (size_t i = 0; i < x_values_4.size() - 1; ++i) {
        if (x_values_4[i] > x_values_4[i + 1]) {
            std::cerr << "Error: Unsorted x values in points_4." << std::endl;
            return false;
        }
    }

    return true;
}

bool InterpolationTester::LinearInterpolationTest() {
    
    bool test_result = true;

    std::cout << "Set 1 test:" << std::endl;

    LinearInterpolation<double> linear_1(points_1);

    std::cout << "Expected value: " << results_1[0] << std::endl;
    std::cout << "Obtained value: " << linear_1(x_query) << std::endl;
    std::cout << "Difference: " << linear_1(x_query) - results_1[0] << std::endl;
    std::cout << "Pct Difference: " << (linear_1(x_query) - results_1[0]) / results_1[0] * 100 << "%" << std::endl;

    if (std::abs((linear_1(x_query) - results_1[0])/results_1[0]) > epsilon) {
        test_result = false;
    }

    std::cout << "Set 2 test:" << std::endl;

    LinearInterpolation<double> linear_2(points_2);

    std::cout << "Expected value: " << results_2[0] << std::endl;
    std::cout << "Obtained value: " << linear_2(x_query) << std::endl;
    std::cout << "Difference: " << linear_2(x_query) - results_2[0] << std::endl;
    std::cout << "Pct Difference: " << (linear_2(x_query) - results_2[0]) / results_2[0] * 100 << "%" << std::endl;

    if (std::abs((linear_2(x_query) - results_2[0])/results_2[0]) > epsilon) {
        test_result = false;
    }

    std::cout << "Set 3 test:" << std::endl;

    LinearInterpolation<double> linear_3(points_3);

    std::cout << "Expected value: " << results_3[0] << std::endl;
    std::cout << "Obtained value: " << linear_3(x_query) << std::endl;
    std::cout << "Difference: " << linear_3(x_query) - results_3[0] << std::endl;
    std::cout << "Pct Difference: " << (linear_3(x_query) - results_3[0]) / results_3[0] * 100 << "%" << std::endl;

    if (std::abs((linear_3(x_query) - results_3[0])/results_3[0]) > epsilon) {
        test_result = false;
    }

    std::cout << "Set 4 test:" << std::endl;

    LinearInterpolation<double> linear_4(points_4);

    std::cout << "Expected value: " << results_4[0] << std::endl;
    std::cout << "Obtained value: " << linear_4(x_query) << std::endl;
    std::cout << "Difference: " << linear_4(x_query) - results_4[0] << std::endl;
    std::cout << "Pct Difference: " << (linear_4(x_query) - results_4[0]) / results_4[0] * 100 << "%" << std::endl;

    if (std::abs((linear_4(x_query) - results_4[0])/results_4[0]) > epsilon) {
        test_result = false;
    }

    return test_result;
    
}

bool InterpolationTester::LagrangeInterpolationTest() {
    
    bool test_results = true;

    std::cout << "Set 1 test:" << std::endl;

    Lagrange<double> lagrange_1(points_1);

    std::cout << "Expected value: " << results_1[1] << std::endl;
    std::cout << "Obtained value: " << lagrange_1(x_query) << std::endl;
    std::cout << "Difference: " << lagrange_1(x_query) - results_1[1] << std::endl;
    std::cout << "Pct Difference: " << (lagrange_1(x_query) - results_1[1]) / results_1[1] * 100 << "%" << std::endl;

    if (std::abs((lagrange_1(x_query) - results_1[1])/results_1[1]) > epsilon) {
        test_results = false;
    }

    std::cout << "Set 2 test:" << std::endl;

    Lagrange<double> lagrange_2(points_2);

    std::cout << "Expected value: " << results_2[1] << std::endl;
    std::cout << "Obtained value: " << lagrange_2(x_query) << std::endl;
    std::cout << "Difference: " << lagrange_2(x_query) - results_2[1] << std::endl;
    std::cout << "Pct Difference: " << (lagrange_2(x_query) - results_2[1]) / results_2[1] * 100 << "%" << std::endl;

    if (std::abs((lagrange_2(x_query) - results_2[1])/results_2[1]) > epsilon) {
        test_results = false;
    }

    std::cout << "Set 3 test:" << std::endl;

    Lagrange<double> lagrange_3(points_3);

    std::cout << "Expected value: " << results_3[1] << std::endl;
    std::cout << "Obtained value: " << lagrange_3(x_query) << std::endl;
    std::cout << "Difference: " << lagrange_3(x_query) - results_3[1] << std::endl;
    std::cout << "Pct Difference: " << (lagrange_3(x_query) - results_3[1]) / results_3[1] * 100 << "%" << std::endl;

    if (std::abs((lagrange_3(x_query) - results_3[1])/results_3[1]) > epsilon) {
        test_results = false;
    }

    std::cout << "Set 4 test:" << std::endl;

    Lagrange<double> lagrange_4(points_4);

    std::cout << "Expected value: " << results_4[1] << std::endl;
    std::cout << "Obtained value: " << lagrange_4(x_query) << std::endl;
    std::cout << "Difference: " << lagrange_4(x_query) - results_4[1] << std::endl;
    std::cout << "Pct Difference: " << (lagrange_4(x_query) - results_4[1]) / results_4[1] * 100 << "%" << std::endl;

    if (std::abs((lagrange_4(x_query) - results_4[1])/results_4[1]) > epsilon) {
        test_results = false;
    }

    return test_results;
}

bool InterpolationTester::NewtonInterpolationTest() {
    bool test_results = true;

    std::cout << "Set 1 test:" << std::endl;

    Newton<double> newton_1(points_1);

    std::cout << "Expected value: " << results_1[2] << std::endl;
    std::cout << "Obtained value: " << newton_1(x_query) << std::endl;
    std::cout << "Difference: " << newton_1(x_query) - results_1[2] << std::endl;
    std::cout << "Pct Difference: " << (newton_1(x_query) - results_1[2]) / results_1[2] * 100 << "%" << std::endl;

    if (std::abs((newton_1(x_query) - results_1[2])/results_1[2]) > epsilon) {
        test_results = false;
    }

    std::cout << "Set 2 test:" << std::endl;

    Newton<double> newton_2(points_2);

    std::cout << "Expected value: " << results_2[2] << std::endl;
    std::cout << "Obtained value: " << newton_2(x_query) << std::endl;
    std::cout << "Difference: " << newton_2(x_query) - results_2[2] << std::endl;
    std::cout << "Pct Difference: " << (newton_2(x_query) - results_2[2]) / results_2[2] * 100 << "%" << std::endl;

    if (std::abs((newton_2(x_query) - results_2[2])/results_2[2]) > epsilon) {
        test_results = false;
    }

    std::cout << "Set 3 test:" << std::endl;

    Newton<double> newton_3(points_3);

    std::cout << "Expected value: " << results_3[2] << std::endl;
    std::cout << "Obtained value: " << newton_3(x_query) << std::endl;
    std::cout << "Difference: " << newton_3(x_query) - results_3[2] << std::endl;
    std::cout << "Pct Difference: " << (newton_3(x_query) - results_3[2]) / results_3[2] * 100 << "%" << std::endl;

    if (std::abs((newton_3(x_query) - results_3[2])/results_3[2]) > epsilon) {
        test_results = false;
    }

    std::cout << "Set 4 test:" << std::endl;

    Newton<double> newton_4(points_4);

    std::cout << "Expected value: " << results_4[2] << std::endl;
    std::cout << "Obtained value: " << newton_4(x_query) << std::endl;
    std::cout << "Difference: " << newton_4(x_query) - results_4[2] << std::endl;
    std::cout << "Pct Difference: " << (newton_4(x_query) - results_4[2]) / results_4[2] * 100 << "%" << std::endl;

    if (std::abs((newton_4(x_query) - results_4[2])/results_4[2]) > epsilon) {
        test_results = false;
    }

    return test_results;
}

bool InterpolationTester::CubicSplineInterpolationTest() {
    bool test_results = true;

    std::cout << "Set 1 test:" << std::endl;

    SplineInterpolation<double> spline_1(points_1);

    std::cout << "Expected value: " << results_1[3] << std::endl;
    std::cout << "Obtained value: " << spline_1(x_query) << std::endl;
    std::cout << "Difference: " << spline_1(x_query) - results_1[3] << std::endl;
    std::cout << "Pct Difference: " << (spline_1(x_query) - results_1[3]) / results_1[3] * 100 << "%" << std::endl; 

    if (std::abs((spline_1(x_query) - results_1[3])/results_1[3]) > epsilon) {
        test_results = false;
    }

    std::cout << "Set 2 test:" << std::endl;

    SplineInterpolation<double> spline_2(points_2);

    std::cout << "Expected value: " << results_2[3] << std::endl;
    std::cout << "Obtained value: " << spline_2(x_query) << std::endl;
    std::cout << "Difference: " << spline_2(x_query) - results_2[3] << std::endl;
    std::cout << "Pct Difference: " << (spline_2(x_query) - results_2[3]) / results_2[3] * 100 << "%" << std::endl;

    if (std::abs((spline_2(x_query) - results_2[3])/results_2[3]) > epsilon) {
        test_results = false;
    }

    std::cout << "Set 3 test:" << std::endl;

    SplineInterpolation<double> spline_3(points_3);

    std::cout << "Expected value: " << results_3[3] << std::endl;
    std::cout << "Obtained value: " << spline_3(x_query) << std::endl;
    std::cout << "Difference: " << spline_3(x_query) - results_3[3] << std::endl;
    std::cout << "Pct Difference: " << (spline_3(x_query) - results_3[3]) / results_3[3] * 100 << "%" << std::endl;

    if (std::abs((spline_3(x_query) - results_3[3])/results_3[3]) > epsilon) {
        test_results = false;
    }

    std::cout << "Set 4 test:" << std::endl;

    SplineInterpolation<double> spline_4(points_4);

    std::cout << "Expected value: " << results_4[3] << std::endl;
    std::cout << "Obtained value: " << spline_4(x_query) - results_4[3] << std::endl;
    std::cout << "Difference: " << spline_4(x_query) - results_4[3] << std::endl;
    std::cout << "Pct Difference: " << (spline_4(x_query) - results_4[3]) / results_4[3] * 100 << "%" << std::endl;

    if (std::abs((spline_4(x_query) - results_4[3])/results_4[3]) > epsilon) {
        test_results = false;
    }

    return test_results;
}


