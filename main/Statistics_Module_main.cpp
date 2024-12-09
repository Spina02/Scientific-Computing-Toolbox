#include "../include/Utilities/ImportCSV.hpp"
#include "../include/Statistics_Module/Utils.hpp"
#include "../include/Statistics_Module/Stats.hpp"

#include <iostream>
#include <filesystem>

using namespace ScientificToolbox;
/**
 * @file Statistics_Module_main.cpp
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
 * - <filesystem>      - For path handling
 * - <Eigen/Dense>     - For matrix operations
 * - "ImportCSV.hpp"   - For CSV file importing
 * - "Stats.hpp"  - For statistical computations
 * - "Utils.hpp"       - For extracting values from columns
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
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
// int main(int argc, char** argv) {

    // std::string inputFile = "../../data/Food_and_Nutrition__.csv";
    // std::string outputFile = "../../output/Statistics_output.txt";
    // std::string targetColumn = "Weight"; 

    

int main(int argc, char** argv) {
    // Default values
    std::string inputFile = "../../data/Food_and_Nutrition__.csv";
    std::string outputFile = "../../output/Statistics_output.txt";
    std::string targetColumn = "Weight"; 

    // Parse command line arguments
    if (argc > 1) {
        inputFile = argv[1];
    }
    if (argc > 2) {
        outputFile = argv[2];
    }
    if (argc > 3) {
        targetColumn = argv[3];
    }

    
    if (argc == 1) {
        std::cout << std::endl;
        std::cout << "Usage: " << argv[0] << " [input_file] [output_file] [target_column]\n";
        std::cout << "Using default values:\n";
        std::cout << "Input file: " << inputFile << "\n";
        std::cout << "Output file: " << outputFile << "\n";
        std::cout << "Target column: " << targetColumn << "\n";
        std::cout << std::endl;
    }

    try {

        ScientificToolbox::ImportCSV importer;
        importer.import(inputFile);
        auto data = importer.getData();


        std::vector<std::string> numericCols;
        std::vector<std::vector<double>> numericData;

        for (const auto& [key, value] : data[0]) {

            if (std::holds_alternative<double>(value.value()) ||
                std::holds_alternative<int>(value.value())) {

                numericCols.push_back(key);
                numericData.push_back(ScientificToolbox::Utils::extractColumn<double>(data, key));
            }
        }

        // create output dir if it doesn't exist
        std::filesystem::create_directories(std::filesystem::path(outputFile).parent_path());

        std::ofstream outFile(outputFile);
        outFile << std::fixed << std::setprecision(2);

        if (!targetColumn.empty()) {
            if (std::find(numericCols.begin(), numericCols.end(), targetColumn) == numericCols.end()) {
                throw std::runtime_error("Column '" + targetColumn + "' does not exist or is not numeric");
            }

            auto columnData = ScientificToolbox::Utils::extractColumn<double>(data, targetColumn);

            outFile << "Statistics for column: " << targetColumn << "\n";
            outFile << "Mean: " << ScientificToolbox::Statistics::mean(columnData) << "\n";
            outFile << "Median: " << ScientificToolbox::Statistics::median(columnData) << "\n";
            outFile << "Variance: " << ScientificToolbox::Statistics::variance(columnData) << "\n";
            outFile << "Standard Deviation: " << ScientificToolbox::Statistics::sd(columnData) << "\n";

            auto freqCount = ScientificToolbox::Statistics::freqCount(columnData);
        
            outFile << "Frequency Count: \n";
            for (const auto& [key, value] : freqCount) {
                outFile << key << ": " << value << "\n";
            }
            outFile << "\n";


        }

        Eigen::MatrixXd dataMat(data.size(), numericCols.size());
        for (size_t i = 0; i < numericCols.size(); ++i) {
            std::vector<double> columnData = ScientificToolbox::Utils::extractColumn<double>(data, numericCols[i]);
            for (size_t j = 0; j < data.size(); ++j) {
                dataMat(j, i) = columnData[j];
            }
        }

        Eigen::MatrixXd correlationMat = ScientificToolbox::Statistics::correlationM(dataMat);

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