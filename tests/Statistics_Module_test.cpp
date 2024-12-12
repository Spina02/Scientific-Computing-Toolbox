#include "../include/Statistics_Module/Stats.hpp"
#include <iostream>
#include <cassert>
#include <cmath>
#include <random>
#include <Eigen/Dense>


using namespace ScientificToolbox::Statistics;

/**
 * @brief Test class for statistical functions validation
 * 
 * This class contains unit tests for various statistical computations including:
 * - Mean calculation
 * - Variance calculation  
 * - Correlation matrix computation
 * 
 * The test uses different data sets for completeness:
 * - Constant values
 * - Ascending sequence
 * - Normally distributed random data
 * - Correlated data matrix
 * 
 * @note Uses Eigen library for matrix operations
 * 
 * Test methods:
 * - test_mean(): Validates mean calculation
 * - test_variance(): Validates variance calculation
 * - test_correlation(): Validates correlation matrix computation
 * 
 * Helper methods:
 * - generateNormalDistribution(): Generates normal distribution test data
 * - approx_equal(): Compares floating point values with tolerance
 * - setUp(): Initializes test data sets
 * 
 * @test Validates statistical computations against known values
 * @see Stats.hpp for the implementation of tested functions
 */
class StatisticsTest {
protected:
     
    
    /**
     * @brief Generates normal distribution test data
     * 
     * @param n Number of data points
     * @param mean Mean value of the distribution
     * @param stddev Standard deviation of the distribution
     * @return Vector of normally distributed data points
     */
    std::vector<double> generateNormalDistribution(size_t n, double mean, double stddev) {
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<> d{mean, stddev};
        std::vector<double> data(n);
        for(auto& x : data) {
            x = d(gen);
        }
        return data;
    }

    bool approx_equal(double a, double b, double epsilon = 1e-1) {
        return std::abs(a - b) < epsilon;
    }

    // Test-data    
    std::vector<double> constant_data;
    std::vector<double> ascending_data;
    std::vector<double> random_normal_data;
    Eigen::MatrixXd correlation_data;



     /**  @brief Initializes test data sets
    */
    void setUp() {
        // Initialize test-data
        constant_data = std::vector<double>(100, 5.0);  // 100 elements of value 5.0
        
        ascending_data.resize(100);
        std::iota(ascending_data.begin(), ascending_data.end(), 1);  // 1 to 100
        
        random_normal_data = generateNormalDistribution(1000, 0.0, 1.0);

        // Create correlation test data with known correlation
        correlation_data.resize(100, 3);
        for(int i = 0; i < 100; ++i) {
            correlation_data(i, 0) = i;
            correlation_data(i, 1) = 2 * i + generateNormalDistribution(1, 0, 0.1)[0];  // Strong positive correlation
            correlation_data(i, 2) = -0.5 * i + generateNormalDistribution(1, 0, 0.1)[0];  // Strong negative correlation
        }
    }

    bool test_mean() {
        bool passed = true;
        
        // Test 1: Constant values
        passed &= approx_equal(mean(constant_data), 5.0);
        
        
        // Test 2: Arithmetic sequence
        passed &= approx_equal(mean(ascending_data), 50.5);
        
        
        // Test 3: Normal distribution
        double sample_mean = mean(random_normal_data);
        passed &= std::abs(sample_mean) < 0.1;  // Should be close to 0
        
        
        //Test 4: Empty vector
        try {
            mean(std::vector<double>{});
            passed = false;  // Should throw
        } catch (const std::invalid_argument&) {
            
            passed = true;
        }
        
        return passed;
    }

    bool test_variance() {
        bool passed = true;
        
        // Test 1: Constant values (variance should be 0)
        passed &= approx_equal(variance(constant_data), 0.0);
        
        
        // Test 2: Known variance of arithmetic sequence
        double expected_variance = 833.25;  // (n^2 - 1) / 12 for n = 100
        passed &= approx_equal(variance(ascending_data), expected_variance, 0.01);
        
        
        // Test 3: Normal distribution (should be close to 1)
        passed &= std::abs(variance(random_normal_data) - 1.0) < 0.1;
        
        
        return passed;
    }

    bool test_correlation() {
        bool passed = true;
        
        Eigen::MatrixXd corr = correlationM(correlation_data);
        
        // Test 1: Diagonal elements should be 1
        for(int i = 0; i < corr.rows(); ++i) {
            passed &= approx_equal(corr(i,i), 1.0);
            
        }
        
        // Test 2: Correlation between column 0 and 1 should be close to 1
        passed &= approx_equal(corr(0,1), 1.0, 0.1);
        
        
        // Test 3: Correlation between column 0 and 2 should be close to -1
        passed &= approx_equal(corr(0,2), -1.0, 0.1);
        
        
        return passed;
    }

public:
    bool run_all_tests() {
        setUp();
        
        bool all_passed = true;
        std::cout << "\n\n\n Running statistics tests \n";
        
        all_passed &= test_mean();
        all_passed &= test_variance();
        all_passed &= test_correlation();
        
        return all_passed;
    }
};

int main() {
    StatisticsTest tester;
    bool all_passed = tester.run_all_tests();
    
    if (all_passed) {
        std::cout << "\nAll statistics tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << "\nSome tests failed!" << std::endl;
        return 1;
    }
}