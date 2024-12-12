#include "../include/Statistics_Module/Stats.hpp"
#include <iostream>
#include <cassert>
#include <cmath>
#include <Eigen/Dense>

using namespace ScientificToolbox::Statistics;

// Helper function. checks if two doubles are approximately equal
bool approx_equal(double a, double b, double epsilon = 1e-2) {
    return std::abs(a - b) < epsilon;
}

void test_mean() {
    std::cout << "Testing mean function..." << std::endl;

    std::vector<double> data1 = {1.0, 2.0, 3.0, 4.0, 5.0};
    assert(approx_equal(mean(data1), 3.0));

    std::vector<int> data2 = {1, 2, 3, 4, 5};
    assert(approx_equal(mean(data2), 3.0));

    std::cout << "Mean tests passed!" << std::endl;
}

void test_median() {
    std::cout << "Testing median function..." << std::endl;

    // computing median for an odd number of elements
    std::vector<double> data1 = {5.0, 2.0, 1.0, 3.0, 4.0};
    assert(approx_equal(median(data1), 3.0));

    // this time for even number of elements
    std::vector<double> data2 = {5.0, 2.0, 1.0, 4.0};
    assert(approx_equal(median(data2), 3.0));

    std::cout << "Median tests passed!" << std::endl;
}

void test_variance() {
    std::cout << "Testing variance function..." << std::endl;

    std::vector<double> data = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    assert(approx_equal(variance(data), 4.57));
    std::cout << "Variance tests passed!" << std::endl;
}

void test_standard_deviation() {
    std::cout << "Testing standard deviation function..." << std::endl;
    std::vector<double> data = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    assert(approx_equal(sd(data), std::sqrt(4.57)));

    std::cout << "Standard deviation tests passed!" << std::endl;
}

void test_frequency_count() {
    std::cout << "Testing frequency count function..." << std::endl;

    std::vector<int> data = {1, 2, 2, 3, 3, 3, 4, 4};
    auto freq = freqCount(data);

    assert(freq[1] == 1);
    assert(freq[2] == 2);
    assert(freq[3] == 3);
    assert(freq[4] == 2);
    std::cout << "Frequency count tests passed!" << std::endl;
}

void test_correlation_matrix() {
    std::cout << "Testing correlation matrix function..." << std::endl;

    
    Eigen::MatrixXd data(3, 3);
    data << 1, 2, 3,
            2, 4, 6,
            3, 6, 9;

    Eigen::MatrixXd corr_matrix = correlationM(data);

    // checking if diagonal elements are 1
    for (int i = 0; i < corr_matrix.rows(); ++i) {
        assert(approx_equal(corr_matrix(i, i), 1.0));
    }

    // Off-diagonal elements should also be 1 due to perfect correlation
    for (int i = 0; i < corr_matrix.rows(); ++i) {
        for (int j = 0; j < corr_matrix.cols(); ++j) {
            if (i != j) {
                assert(approx_equal(corr_matrix(i, j), 1.0));
            }
        }
    }

    std::cout << "Correlation matrix tests passed!" << std::endl;
}

int main() {
    test_mean();
    test_median();
    test_variance();
    test_standard_deviation();
    test_frequency_count();
    test_correlation_matrix();

    std::cout << "\nAll statistics tests passed!" << std::endl;

    return 0;
}