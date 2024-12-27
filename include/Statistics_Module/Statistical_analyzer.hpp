#ifndef STATISTICAL_ANALYZER_HPP
#define STATISTICAL_ANALYZER_HPP

#include "Dataset.hpp"
#include <Eigen/Dense>

namespace ScientificToolbox::Statistics {

class StatisticalAnalyzer {
public:
    explicit StatisticalAnalyzer(std::shared_ptr<Dataset> dataset);

    template<typename T>
    double mean(const std::string& columnName) const;
    
    template<typename T>
    double median(const std::string& columnName) const;
    
    template<typename T>
    double variance(const std::string& columnName) const;
    
    template<typename T>
    double standardDeviation(const std::string& columnName) const;
    
    template<typename T>
    std::unordered_map<T, size_t> frequencyCount(const std::string& columnName) const;
    
    Eigen::MatrixXd correlationMatrix(const std::vector<std::string>& columnNames) const;
    void reportStrongCorrelations(const std::vector<std::string>& columnNames, 
                                double threshold = 0.7,
                                std::ostream& outStream = std::cout) const;

private:
    std::shared_ptr<Dataset> dataset;
};

} // namespace ScientificToolbox::Statistics

#endif