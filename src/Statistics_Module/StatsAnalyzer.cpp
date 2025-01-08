#include "../../include/Statistics_Module/Statistical_analyzer.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>

namespace ScientificToolbox::Statistics {

StatisticalAnalyzer::StatisticalAnalyzer(std::shared_ptr<Dataset> ds) : dataset(ds) {

    if (!dataset) {

        throw std::invalid_argument("Dataset is empty");
    }

}

template<typename T>    
double StatisticalAnalyzer::mean(const std::string& ColumnName) const{

    auto data = dataset->getColumn<T>(ColumnName);
    if (data.empty()) {
        throw std::invalid_argument("Cannot compute mean of a column that does not exist");
    }
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

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
    return data[data.size() / 2];}

template<typename  T>

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

template<typename T>
double StatisticalAnalyzer::standardDeviation(const std::string& ColumnName) const {

    return std::sqrt(variance<T>(ColumnName));
}

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
    return freqMap;}




Eigen::MatrixXd StatisticalAnalyzer::correlationMatrix(const std::vector<std::string>& columnNames) const {
    if (columnNames.empty()) {
        throw std::invalid_argument("No columns specified for correlation analysis");
    }

    size_t n = columnNames.size();
    size_t rows = dataset->size();
    
    Eigen::MatrixXd dataMatrix(rows, n);
    
    for (size_t j = 0; j < n; ++j) {
        std::vector<double> colData = dataset->getColumn<double>(columnNames[j]);
        for (size_t i = 0; i < rows; ++i) {
            dataMatrix(i, j) = colData[i];
        }
    }

    Eigen::MatrixXd centered = dataMatrix.rowwise() - dataMatrix.colwise().mean();
    
    Eigen::MatrixXd cov = (centered.adjoint() * centered) / double(rows - 1);
    
    Eigen::VectorXd stdDev = cov.diagonal().array().sqrt();
    
    return cov.array() / (stdDev * stdDev.transpose()).array();
}




void StatisticalAnalyzer::reportStrongCorrelations(const std::vector<std::string>& columnNames, double threshold,std::ostream& outStream) const {
    
    Eigen::MatrixXd corrMatrix = correlationMatrix(columnNames);
    
    // if (corrMatrix.rows() != columnNames.size() || corrMatrix.cols() != columnNames.size()) {
    //     throw std::runtime_error("Correlation matrix dimensions do not match column names");
    // }
    
    outStream << "Strong Correlations (|correlation| > " << threshold << "):\n";
    
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

}