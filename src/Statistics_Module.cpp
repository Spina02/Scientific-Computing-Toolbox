#include "../include/Statistics_Module.hpp"
#include <vector>
#include <iostream>

double mean(const std::vector<double>* data){
    double sum = 0;
    for (int i = 0; i < data->size(); i++){
        sum += data->at(i);
    }
    return sum / data->size();
}

double median(const std::vector<double>* data){
    double median;
    if (data->size() % 2 == 0){
        median = (data->at(data->size() / 2) + data->at(data->size() / 2 - 1)) / 2;
    } else {
        median = data->at(data->size() / 2);
    }
    return median;
}

double sd(const std::vector<double>* data){
    double mean_val = mean(data);
    double sum = 0;
    for (int i = 0; i < data->size(); i++){
        sum += pow(data->at(i) - mean_val, 2);
    }
    return sqrt(sum / data->size());
}

double variance(const std::vector<double>* data){
    double mean_val = mean(data);
    double sum = 0;
    for (int i = 0; i < data->size(); i++){
        sum += pow(data->at(i) - mean_val, 2);
    }
    return sum / data->size();
}

double frequency_count(const std::vector<double>* data, double value){
    double count = 0;
    for (int i = 0; i < data->size(); i++){
        if (data->at(i) == value){
            count++;
        }
    }
    return count;
}

std::vector<std::vector<double>> correlation_matrix(const std::vector<std::vector<double>>* data){
    std::vector<std::vector<double>> correlation_matrix;
    for (int i = 0; i < data->size(); i++){
        std::vector<double> row;
        for (int j = 0; j < data->size(); j++){
            double sum = 0;
            double sum_x = 0;
            double sum_y = 0;
            double sum_x_sq = 0;
            double sum_y_sq = 0;
            for (int k = 0; k < data->at(i).size(); k++){
                sum += data->at(i).at(k) * data->at(j).at(k);
                sum_x += data->at(i).at(k);
                sum_y += data->at(j).at(k);
                sum_x_sq += pow(data->at(i).at(k), 2);
                sum_y_sq += pow(data->at(j).at(k), 2);
            }
            double n = data->at(i).size();
            double numerator = n * sum - sum_x * sum_y;
            double denominator = sqrt((n * sum_x_sq - pow(sum_x, 2)) * (n * sum_y_sq - pow(sum_y, 2)));
            row.push_back(numerator / denominator);
        }
        correlation_matrix.push_back(row);
    }
    return correlation_matrix;
}


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