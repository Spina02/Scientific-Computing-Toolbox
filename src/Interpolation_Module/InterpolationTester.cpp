
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

bool InterpolationTester::run_tests() {
    if (!import_data()) {
        return false;
    }

    if (!check_data()) {
        return false;
    }

    if (!test_linear_interpolation()) {
        return false;
    }

    if (!test_cubic_spline_interpolation()) {
        return false;
    }

    if (!test_lagrange_interpolation()) {
        return false;
    }

    if (!test_newton_interpolation()) {
        return false;
    }

    return true;
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

bool InterpolationTester::test_linear_interpolation() {
    LinearInterpolation<double> linear_1(points_1);
    LinearInterpolation<double> linear_2(points_2);
    LinearInterpolation<double> linear_3(points_3);
    LinearInterpolation<double> linear_4(points_4);

    if (linear_1(x_query) - (results_1[0]) > epsilon){
        std::cerr << "Error: Linear interpolation test failed for points_1." << std::endl;
        return false;
    }

    if (linear_2(x_query) - (results_2[0]) > epsilon){
        std::cerr << "Error: Linear interpolation test failed for points_2." << std::endl;
        return false;
    }

    if (linear_3(x_query) - (results_3[0]) > epsilon){
        std::cerr << "Error: Linear interpolation test failed for points_3." << std::endl;
        return false;
    }

    if (linear_4(x_query) - (results_4[0]) > epsilon){
        std::cerr << "Error: Linear interpolation test failed for points_4." << std::endl;
        return false;
    }

    return true;
}

bool InterpolationTester::test_lagrange_interpolation() {
    Lagrange<double> lagrange_1(points_1);
    Lagrange<double> lagrange_2(points_2);
    Lagrange<double> lagrange_3(points_3);
    Lagrange<double> lagrange_4(points_4);

    if (lagrange_1(x_query) - (results_1[1]) > epsilon){
        std::cerr << "Error: Lagrange interpolation test failed for points_1." << std::endl;
        return false;
    }

    if (lagrange_2(x_query) - (results_2[1]) > epsilon){
        std::cerr << "Error: Lagrange interpolation test failed for points_2." << std::endl;
        return false;
    }

    if (lagrange_3(x_query) - (results_3[1]) > epsilon){
        std::cerr << "Error: Lagrange interpolation test failed for points_3." << std::endl;
        return false;
    }

    if (lagrange_4(x_query) - (results_4[1]) > epsilon){
        std::cerr << "Error: Lagrange interpolation test failed for points_4." << std::endl;
        return false;
    }

    return true;
}

bool InterpolationTester::test_newton_interpolation() {
    Newton<double> newton_1(points_1);
    Newton<double> newton_2(points_2);
    Newton<double> newton_3(points_3);
    Newton<double> newton_4(points_4);

    if (newton_1(x_query) - (results_1[2]) > epsilon){
        std::cerr << "Error: Newton interpolation test failed for points_1." << std::endl;
        return false;
    }

    if (newton_2(x_query) - (results_2[2]) > epsilon){
        std::cerr << "Error: Newton interpolation test failed for points_2." << std::endl;
        return false;
    }

    if (newton_3(x_query) - (results_3[2]) > epsilon){
        std::cerr << "Error: Newton interpolation test failed for points_3." << std::endl;
        return false;
    }

    if (newton_4(x_query) - (results_4[2]) > epsilon){
        std::cerr << "Error: Newton interpolation test failed for points_4." << std::endl;
        return false;
    }

    return true;
}

bool InterpolationTester::test_cubic_spline_interpolation() {
    SplineInterpolation<double> spline_1(points_1);
    SplineInterpolation<double> spline_2(points_2);
    SplineInterpolation<double> spline_3(points_3);
    SplineInterpolation<double> spline_4(points_4);

    if (spline_1(x_query) - (results_1[3]) > epsilon){
        std::cerr << "Error: Cubic spline interpolation test failed for points_1." << std::endl;
        return false;
    }

    if (spline_2(x_query) - (results_2[3]) > epsilon){
        std::cerr << "Error: Cubic spline interpolation test failed for points_2." << std::endl;
        return false;
    }

    if (spline_3(x_query) - (results_3[3]) > epsilon){
        std::cerr << "Error: Cubic spline interpolation test failed for points_3." << std::endl;
        return false;
    }

    if (spline_4(x_query) - (results_4[3]) > epsilon){
        std::cerr << "Error: Cubic spline interpolation test failed for points_4." << std::endl;
        return false;
    }

    return true;
}
