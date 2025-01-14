#ifndef STATISTICAL_ANALYZER_HPP
#define STATISTICAL_ANALYZER_HPP

#include "Dataset.hpp"
#include <Eigen/Dense>
namespace ScientificToolbox::Statistics {
/**
 * @brief A class for performing statistical analysis on datasets
 * 
 * The StatisticalAnalyzer class provides various statistical operations and analysis
 * tools for processing data stored in a Dataset object. It supports both numerical
 * and (basic) categorical data analysis through templated methods.
 * 
 * This class offers following statistical operations:
 * - Basic statistical measures (mean, median, variance, standard deviation)
 * - Frequency analysis for categorical data
 * - Correlation analysis between multiple variables
 * 
 * 
 * 
 * @see Dataset
 */
class StatisticalAnalyzer {
public:
    /**
     * @brief Constructs a StatisticalAnalyzer with a shared pointer to a Dataset
     * @param dataset Shared pointer to the Dataset to be analyzed
     */
    explicit StatisticalAnalyzer(std::shared_ptr<Dataset> dataset);

    /**
     * @brief Calculates the arithmetic mean of a specified column
     * @tparam T Data type of the column
     * @param columnName Name of the column to analyze
     * @return Double representing the mean value
     * @throws std::invalid_argument if column doesn't exist or type mismatch
     */
    template<typename T>
    double mean(const std::string& columnName) const;
    
    /**
     * @brief Calculates the median value of a specified column
     * @tparam T Data type of the column
     * @param columnName Name of the column to analyze
     * @return Double representing the median value
     * @throws std::invalid_argument if column doesn't exist or type mismatch
     */
    template<typename T>
    double median(const std::string& columnName) const;
    
    /**
     * @brief Calculates the variance of a specified column
     * @tparam T Data type of the column
     * @param columnName Name of the column to analyze
     * @return Double representing the variance
     * @throws std::invalid_argument if column doesn't exist or type mismatch
     */
    template<typename T>
    double variance(const std::string& columnName) const;
    
    /**
     * @brief Calculates the standard deviation of a specified column
     * @tparam T Data type of the column
     * @param columnName Name of the column to analyze
     * @return Double representing the standard deviation
     * @throws std::invalid_argument if column doesn't exist or type mismatch
     */
    template<typename T>
    double standardDeviation(const std::string& columnName) const;
    
    /**
     * @brief Computes frequency distribution of values in a specified column
     * @tparam T Data type of the column
     * @param columnName Name of the column to analyze
     * @return Unordered map containing value-frequency pairs
     * @throws std::invalid_argument if column doesn't exist or type mismatch
     */
    template<typename T>
    std::unordered_map<T, size_t> frequencyCount(const std::string& columnName) const;
    
    /**
     * @brief Computes the correlation matrix for specified columns
     * @param columnNames Vector of column names to include in correlation analysis
     * @return Eigen::MatrixXd containing the correlation coefficients
     * @throws std::invalid_argument if any column doesn't exist or contains non-numeric data
     */
    Eigen::MatrixXd correlationMatrix(const std::vector<std::string>& columnNames) const;

    /**
     * @brief Reports pairs of columns with correlation coefficients exceeding the threshold
     * @param columnNames Vector of column names to analyze
     * @param threshold Correlation coefficient threshold (default: 0.7)
     * @param outStream Output stream to write the report to (default: std::cout)
     * @throws std::invalid_argument if any column doesn't exist or contains non-numeric data
     */
    void reportStrongCorrelations(const std::vector<std::string>& columnNames, 
                                double threshold = 0.7,
                                std::ostream& outStream = std::cout) const;
private:
    std::shared_ptr<Dataset> dataset;
};

} // namespace ScientificToolbox::Statistics

#endif