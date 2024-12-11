
#include "../../include/Interpolation_Module/DataTester.hpp"
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