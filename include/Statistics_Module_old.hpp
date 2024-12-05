#ifndef Statistics_Module_HPP
#define Statistics_Module_HPP

// Setting up functions ONLY for numerical cleaned data
// MUST be generalised later for any data type

#include <vector>
#include <iostream>
#include <cmath>

double mean(const std::vector<double>* data);
double median(const std::vector<double>* data);
double sd(const std::vector<double>* data);
double variance(const std::vector<double>* data);
double frequency_count(const std::vector<double>* data, double value);
std::vector<std::vector<double>> correlation_matrix(const std::vector<std::vector<double>>* data);

#endif
