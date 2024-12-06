#include "../include/Statistics_Module/ImportCSV.hpp"
#include "../include/Statistics_Module/Utils.hpp"
#include "../include/Statistics_Module/Stats.hpp"

#include <iostream>
#include <filesystem>


/**
 * @file StatMod_main.cpp
 * @brief Main program for statistical analysis of CSV data
 * 
 * This program performs statistical analysis on numeric data from CSV files.
 * It calculates descriptive statistics and correlations between numeric columns.
 * 
 * Required includes:
 * - <iostream>          - For standard I/O operations
 * - <fstream>          - For file operations
 * - <string>           - For string handling
 * - <vector>           - For dynamic arrays
 * - <algorithm>        - For std::find
 * - <iomanip>         - For output formatting
 * - <filesystem>      - For path handling
 * - <Eigen/Dense>     - For matrix operations
 * - "GetPot"          - For command line argument parsing
 * - "ImportCSV.hpp"   - For CSV file importing
 * - "Statistics.hpp"  - For statistical computations
 * - "Utils.hpp"       - For utility functions
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * 
 * Command line options:
 * -i : Input CSV file path (default: "data.csv")
 * -o : Output text file path (default: "output.txt")
 * -c : Target column name for specific analysis (optional)
 * 
 * The program:
 * 1. Parses command line arguments
 * 2. Imports CSV data
 * 3. Identifies numeric columns
 * 4. If target column specified:
 *    - Calculates mean, median, variance, standard deviation
 *    - Generates frequency count
 * 5. Calculates correlation matrix for all numeric columns
 * 6. Reports strong correlations (|correlation| > 0.7)
 * 7. Saves all results to specified output file
 * 
 * @return 0 on success, prints error message to stderr on failure
 * 
 * @throws std::runtime_error If target column doesn't exist or isn't numeric
 * @throws Various exceptions from file operations and data processing
 */
int main(int argc, char** argv) {

    std::string inputFile = "../../data/detailed_meals_macros_CLEANED.csv";
    std::string outputFile = "../../output/Statistics_output.txt";
    std::string targetColumn = "Ages"; 

    // GetPot cmdline(argc, argv);
    //std::string inputFile = cmdline.follow("../../data/detailed_meals_macros.csv", "-i");
    // std::string outputFile = cmdline.follow("../../output/Statistics_output.txt", "-o");
    //std::string targetColumn = cmdline.follow("Ages", "-c");



    try {

        StatsModule::ImportCSV importer;
        importer.import(inputFile);
        auto data = importer.getData();


        std::vector<std::string> numericCols;
        std::vector<std::vector<double>> numericData;

        for (const auto& [key, value] : data[0]) {

            if (std::holds_alternative<double>(value.value()) ||
                std::holds_alternative<int>(value.value())) {

                numericCols.push_back(key);
                numericData.push_back(StatsModule::Utils::extractColumn<double>(data, key));
            }
        }



        std::ofstream outFile(outputFile);
        outFile << std::fixed << std::setprecision(2);

        if (!targetColumn.empty()) {
            if (std::find(numericCols.begin(), numericCols.end(), targetColumn) == numericCols.end()) {
                throw std::runtime_error("Column '" + targetColumn + "' does not exist or is not numeric");
            }

            auto columnData = StatsModule::Utils::extractColumn<double>(data, targetColumn);

            outFile << "Statistics for column: " << targetColumn << "\n";
            outFile << "Mean: " << StatsModule::Statistics::mean(columnData) << "\n";
            outFile << "Median: " << StatsModule::Statistics::median(columnData) << "\n";
            outFile << "Variance: " << StatsModule::Statistics::variance(columnData) << "\n";
            outFile << "Standard Deviation: " << StatsModule::Statistics::sd(columnData) << "\n";

            auto freqCount = StatsModule::Statistics::freqCount(columnData);
        
            outFile << "Frequency Count: \n";
            for (const auto& [key, value] : freqCount) {
                outFile << key << ": " << value << "\n";
            }
            outFile << "\n";


        }

        Eigen::MatrixXd dataMat(data.size(), numericCols.size());
        for (size_t i = 0; i < numericCols.size(); ++i) {
            std::vector<double> columnData = StatsModule::Utils::extractColumn<double>(data, numericCols[i]);
            for (size_t j = 0; j < data.size(); ++j) {
                dataMat(j, i) = columnData[j];
            }
        }

        Eigen::MatrixXd correlationMat = StatsModule::Statistics::correlationM(dataMat);

        outFile << "Strong Correlations (|correlation| > 0.7): \n";
        for (int i = 0; i < correlationMat.rows(); i++) {
            for (int j = 0; j < correlationMat.cols(); j++) {
                if (i != j && std::abs(correlationMat(i,j)) > 0.7) {
                     outFile << numericCols[i] << " - " << numericCols[j]  << ": " << correlationMat(i,j) << "\n";
                    }
                }
        }

        

        std::cout << "Stat results saved to " << std::filesystem::current_path() / outputFile << std::endl;

    } catch (const std::exception& excep) {

            std::cerr <<"error:" << excep.what() << std::endl;
    }

    return 0;


}