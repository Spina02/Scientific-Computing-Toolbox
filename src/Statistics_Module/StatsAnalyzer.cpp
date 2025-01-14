#include "../../include/Statistics_Module/Statistical_analyzer.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>

namespace ScientificToolbox::Statistics {

/**
 * @brief Constructor for StatisticalAnalyzer
 * @param ds Shared pointer to a Dataset object
 * @throws std::invalid_argument if dataset is empty
 */
StatisticalAnalyzer::StatisticalAnalyzer(std::shared_ptr<Dataset> ds) : dataset(ds) {
    if (!dataset) {
        throw std::invalid_argument("Dataset is empty");
    }
}

/**
 * @brief Calculates the arithmetic mean of a column
 * @tparam T Data type of the column
 * @param ColumnName Name of the column to analyze
 * @return Double value representing the mean
 * @throws std::invalid_argument if column doesn't exist
 */
template<typename T>    
double StatisticalAnalyzer::mean(const std::string& ColumnName) const{
    auto data = dataset->getColumn<T>(ColumnName);
    if (data.empty()) {
        throw std::invalid_argument("Cannot compute mean of a column that does not exist");
    }
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

/**
 * @brief Calculates the median value of a column
 * @tparam T Data type of the column
 * @param ColumnName Name of the column to analyze
 * @return Double value representing the median
 * @throws std::invalid_argument if column doesn't exist
 */
template<typename T>
double StatisticalAnalyzer::median(const std::string& ColumnName) const {
    auto data = dataset->getColumn<T>(ColumnName);
    if (data.empty()) {
        throw std::invalid_argument("Cannot compute median of a column that does not exist");
    }
    std::sort(data.begin(), data.end());
    if (data.size() % 2 == 0) {
        return (data[data.size() / 2 - 1] + data[data.size() / 2]) / 2.0;
    }
    return data[data.size() / 2];
}

/**
 * @brief Calculates the variance of a column
 * @tparam T Data type of the column
 * @param ColumnName Name of the column to analyze
 * @return Double value representing the variance
 * @throws std::invalid_argument if column doesn't exist
 */
template<typename T>
double StatisticalAnalyzer::variance(const std::string& ColumnName) const {
    auto data = dataset->getColumn<T>(ColumnName);
    if (data.empty()) {
        throw std::invalid_argument("Cannot compute variance of a column that does not exist");
    }
    double m = mean<T>(ColumnName);
    double accum = 0.0;
    for (const auto& val : data) {
        accum += (val - m) * (val - m);
    }
    return accum / data.size();
}

/**
 * @brief Calculates the standard deviation of a column
 * @tparam T Data type of the column
 * @param ColumnName Name of the column to analyze
 * @return Double value representing the standard deviation
 */
template<typename T>
double StatisticalAnalyzer::standardDeviation(const std::string& ColumnName) const {
    return std::sqrt(variance<T>(ColumnName));
}

/**
 * @brief Counts the frequency of each unique value in a column
 * @tparam T Data type of the column
 * @param ColumnName Name of the column to analyze
 * @return Unordered map with values as keys and their frequencies as values
 * @throws std::invalid_argument if column doesn't exist
 */
template<typename T>
std::unordered_map<T, size_t> StatisticalAnalyzer::frequencyCount(const std::string& ColumnName) const {
    auto data = dataset->getColumn<T>(ColumnName);
    if (data.empty()) {
        throw std::invalid_argument("Cannot compute frequency count of a column that does not exist");
    }
    std::unordered_map<T, size_t> freqMap;
    for (const auto& val : data) {
        freqMap[val]++;
    }
    return freqMap;
}

/**
 * @brief Calculates the correlation matrix for multiple columns
 * @param columnNames Vector of column names to analyze
 * @return Eigen::MatrixXd containing the correlation coefficients
 * @throws std::invalid_argument if no columns are specified
 */
Eigen::MatrixXd StatisticalAnalyzer::correlationMatrix(const std::vector<std::string>& columnNames) const {
    if (columnNames.empty()) {
        throw std::invalid_argument("No columns specified for correlation analysis");
    }

    size_t n = columnNames.size();
    size_t rows = dataset->size();
    
    // Create matrix from data columns
    Eigen::MatrixXd dataMatrix(rows, n);
    for (size_t j = 0; j < n; ++j) {
        std::vector<double> colData = dataset->getColumn<double>(columnNames[j]);
        for (size_t i = 0; i < rows; ++i) {
            dataMatrix(i, j) = colData[i];
        }
    }

    // Center the data
    Eigen::MatrixXd centered = dataMatrix.rowwise() - dataMatrix.colwise().mean();
    
    // Compute covariance matrix
    Eigen::MatrixXd cov = (centered.adjoint() * centered) / double(rows - 1);
    
    // Compute standard deviations
    Eigen::VectorXd stdDev = cov.diagonal().array().sqrt();
    
    // Return correlation matrix
    return cov.array() / (stdDev * stdDev.transpose()).array();
}

/**
 * @brief Reports correlations above a certain threshold
 * @param columnNames Vector of column names to analyze
 * @param threshold Minimum absolute correlation value to report
 * @param outStream Output stream to write results
 */
void StatisticalAnalyzer::reportStrongCorrelations(const std::vector<std::string>& columnNames, 
                                                  double threshold,
                                                  std::ostream& outStream) const {
    Eigen::MatrixXd corrMatrix = correlationMatrix(columnNames);
    
    outStream << "Strong Correlations (|correlation| > " << threshold << "):\n";
    
    // Report correlations above threshold
    for (int i = 0; i < corrMatrix.rows(); i++) {
        for (int j = i + 1; j < corrMatrix.cols(); j++) {
            double correlation = corrMatrix(i, j);
            if (std::abs(correlation) > threshold) {
                outStream << columnNames[i] << " - " << columnNames[j] 
                         << ": " << correlation << "\n";
            }
        }
    }
}


template double StatisticalAnalyzer::mean<double>(const std::string&) const;
template double StatisticalAnalyzer::median<double>(const std::string&) const;
template double StatisticalAnalyzer::variance<double>(const std::string&) const;
template double StatisticalAnalyzer::standardDeviation<double>(const std::string&) const;
template std::unordered_map<double, size_t> StatisticalAnalyzer::frequencyCount<double>(const std::string&) const;
template std::unordered_map<std::string, size_t> StatisticalAnalyzer::frequencyCount<std::string>(const std::string&) const;

}