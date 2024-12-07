#include "../../include/Statistics_Module/Stats.hpp"
#include <vector>
#include <iostream>

double mean(const std::vector<double>* data){
    double sum = 0;
    for (long unsigned int i = 0; i < data->size(); i++){
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
    for (long unsigned int i = 0; i < data->size(); i++){
        sum += pow(data->at(i) - mean_val, 2);
    }
    return sqrt(sum / data->size());
}

double variance(const std::vector<double>* data){
    double mean_val = mean(data);
    double sum = 0;
    for (long unsigned int i = 0; i < data->size(); i++){
        sum += pow(data->at(i) - mean_val, 2);
    }
    return sum / data->size();
}

double frequency_count(const std::vector<double>* data, double value){
    double count = 0;
    for (long unsigned int i = 0; i < data->size(); i++){
        if (data->at(i) == value){
            count++;
        }
    }
    return count;
}

std::vector<std::vector<double>> correlation_matrix(const std::vector<std::vector<double>>* data){
    std::vector<std::vector<double>> correlation_matrix;
    for (long unsigned int i = 0; i < data->size(); i++){
        std::vector<double> row;
        for (long unsigned int j = 0; j < data->size(); j++){
            double sum = 0;
            double sum_x = 0;
            double sum_y = 0;
            double sum_x_sq = 0;
            double sum_y_sq = 0;
            for (long unsigned int k = 0; k < data->at(i).size(); k++){
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