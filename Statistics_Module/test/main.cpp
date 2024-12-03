#include "Statistics_Module.hpp"
#include <vector>
#include <iostream>

// testing functionalities
int main(){
    // squared matrix 10x10 with random values
    std::vector<std::vector<double>> data;
    for (int i = 0; i < 10; i++){
        std::vector<double> row;
        for (int j = 0; j < 10; j++){
            row.push_back(rand() % 100);
        }
        data.push_back(row);
    }

    // testing functionalities except correlation_matrix for first column
    std::vector<double> first_column;
    for (int i = 0; i < 10; i++){
        first_column.push_back(data.at(i).at(0));
    }
    std::cout << "Mean: " << mean(&first_column) << std::endl;
    std::cout << "Median: " << median(&first_column) << std::endl;
    std::cout << "Standard Deviation: " << sd(&first_column) << std::endl;
    std::cout << "Variance: " << variance(&first_column) << std::endl;
    std::cout << "Frequency Count: " << frequency_count(&first_column, 5) << std::endl;

    // testing correlation_matrix
    std::vector<std::vector<double>> correlation_matrix_result = correlation_matrix(&data);
    for (int i = 0; i < correlation_matrix_result.size(); i++){
        for (int j = 0; j < correlation_matrix_result.at(i).size(); j++){
            std::cout << correlation_matrix_result.at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }   
    return 0;
}