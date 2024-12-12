#ifndef STATS_HPP
#define STATS_HPP

#include <vector>
#include <unordered_map>
#include <optional>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <Eigen/Dense>

/**
 * @namespace ScientificToolbox::Statistics
 * @brief Statistical anlysis module providing basic statistical functions
 * 
 * This namespace contains template functions for common statistical calculations:
 * - Descriptive statistical measures (mean, median)
 * - Variability measures (variance, standard deviation)
 * - Frequency analysis
 * - Correlation analysis
 */
namespace ScientificToolbox::Statistics {      
//https://stackoverflow.com/questions/11358425/is-there-a-better-way-to-express-nested-namespaces-in-c-within-the-header


/** ### mean
 * @brief Calculates arithmetic mean of a numeric vector
 * @tparam T Numeric type (must be arithmetic)
 * @param data Input vector of numeric values
 * @return Double precision arithmetic mean
 * @throws static_assert if T is not an arithmetic type
 */
template <typename T>
double mean(const std::vector<T>& data) {

    static_assert(std::is_arithmetic<T>::value, "Must insert numeric type.");
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();

}

/** ### median
 * @brief Calculates median of a numeric vector
 * @tparam T Numeric type (must be arithmetic)
 * @param data Input vector of numeric values
 * @return Double precision median value
 * @throws static_assert if T is not an arithmetic type
 * @throws std::domain_error if input vector is empty
 * @note sort function modifies input vector
 */
template <typename T>
double median(std::vector<T> data) {

    static_assert(std::is_arithmetic<T>::value, "Must insert numeric type.");
    size_t size = data.size();

    if (size==0){

        throw std::domain_error("median of an empty vector is undefined");

    }

    std::sort(data.begin(), data.end());
    if (size % 2 == 0) {
        return (data[size /2 -1] + data[size / 2]) / 2;
    } else {
        return data[size / 2];
    }

}

/** ### variance
 * @brief Calculates sample variance of a numeric vector
 * @tparam T Numeric type (must be arithmetic)
 * @param data Input vector of numeric values
 * @return Double precision sample variance
 * @note Uses n-1 denominator (sample variance)
 */
template <typename T>
double variance(const std::vector<T>& data) {

    double m = mean(data);
    double accum = 0.0;

    for (const auto& val : data) {
        accum += (val - m) * (val-m);

    }
    return accum / (data.size() - 1);
}


/** ### sd
 * @brief Calculates sample standard deviation of a numeric vector
 * @tparam T Numeric type (must be arithmetic)
 * @param data Input vector of numeric values
 * @return Double precision sample standard deviation
 * @note Computed as square root of sample variance
 */
template <typename T>
double sd( const std::vector<T>& data ){

    return std::sqrt(variance(data));
}


/** ### freqCount
 * @brief Counts frequency of each unique value in a vector
 * @tparam T Type of vector elements
 * @param data Input vector
 * @return Unordered map with elements as keys and their frequencies as values
 */
template <typename T>

std::unordered_map<T, size_t> freqCount(const std::vector<T>& data) {

    std::unordered_map<T, size_t> freqMap;

    for (const auto& val : data ) {
        freqMap[val]++;
    }   
    return freqMap;
}


/** ### correlationM
 * @brief Computes correlation matrix from data matrix
 * @param dataMatrix Input matrix where each column represents a variable and each row an observation
 * @return Correlation matrix (symmetric matrix of correlation coefficients)
 * @note Uses Eigen library for matrix operations
 */
Eigen::MatrixXd correlationM( const Eigen::MatrixXd&  dataMatrix) {

    Eigen::MatrixXd centered = dataMatrix.rowwise() - dataMatrix.colwise().mean();  //https://stackoverflow.com/questions/15138634/eigen-is-there-an-inbuilt-way-to-calculate-sample-covariance
    Eigen::MatrixXd cov = (centered.adjoint() * centered) / double(dataMatrix.rows() -1);

    Eigen::VectorXd stand_dev = cov.diagonal().array().sqrt();;
    Eigen::MatrixXd corr_coeff = cov.array() / (stand_dev * stand_dev.transpose()).array();
    return corr_coeff;
}

/** reportStrongCorr
 * @brief Reports correlations stronger than a threshold
 * @param correlationMatrix Correlation matrix of the numeric variables to analyze
 * @param columnNames Vector of names of the numeric columns
 * @param threshold Threshold value for correlation coefficient (absolute value)
 * @param outStream Output stream to write the report
 * @return void
 */
inline void reportStrongCorr(
    const Eigen::MatrixXd& correlationMatrix,
    const std::vector<std::string>& columnNames,
    double threshold = 0.7,
    std::ostream& outStream = std::cout) {

    outStream << "Strong Correlations (|correlation| > " << threshold << "): \n";
    for (int i = 0; i < correlationMatrix.rows();i++) {
        for (int j = 0; j < correlationMatrix.cols(); j++) {
            if (i != j && std::abs(correlationMatrix(i,j)) > threshold) {
                outStream << columnNames[i] << " - " << columnNames[j] << ": " << correlationMatrix(i,j) << "\n";
            }
        }
    


    }


}

}

#endif