#include "../include/Statistics.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <iomanip>

using namespace ScientificToolbox;

int main(int argc, char** argv) {
    // Default values
    std::string inputFile = "../../data/Food_and_Nutrition__.csv";
    std::string outputFile = "../../output/Statistics_output.txt";
    std::vector<std::string> targetColumns = {"Weight","Height"}; 

    // Parse command line arguments
    if (argc > 1) {
        inputFile = argv[1];
    }
    if (argc > 2) {
        outputFile = argv[2];
    }
    if (argc > 3) {
    
    std::string targetsArg = argv[3];
    
    // Checking for spaces between commas
    for (size_t i = 0; i < targetsArg.length() - 1; i++) {
        if (targetsArg[i] == ',' && targetsArg[i + 1] == ' ') {
            throw std::runtime_error("Invalid format: No spaces allowed between commas in target columns list");
        }
    }
    
    std::stringstream ss(targetsArg);
    std::string column;
    targetColumns.clear();
    while (std::getline(ss, column, ',')) {
        targetColumns.push_back(column);
        }
    }

    if (argc == 1) {
        std::cout << "\nStatistics Module Usage:\n";
        std::cout << "------------------------\n";
        std::cout << argv[0] << " [input_file] [output_file] [target_columns]\n\n";
        std::cout << "Parameters:\n";
        std::cout << "  input_file     : Path to input CSV file\n";
        std::cout << "  output_file    : Path for output statistics file\n";
        std::cout << "  target_columns : Comma-separated list of columns to analyze\n\n";
        std::cout << "Current (default) settings:\n";
        std::cout << "  Input file     : " << inputFile << "\n";
        std::cout << "  Output file    : " << outputFile << "\n";
        std::cout << "  Target columns : ";
        std::cout << "\"";
        for (size_t i = 0; i < targetColumns.size(); ++i) {
            std::cout << targetColumns[i] ;
            if (i < targetColumns.size() - 1) std::cout << ",";
        }
        std::cout << "\"\n";
        std::cout << "\n\n";
    }


    try {
        ImportCSV importer;
        importer.import(inputFile);
        auto data = importer.getData();

        std::vector<std::string> numericCols;
        std::vector<std::vector<double>> numericData;

        for (const auto& [key, value] : data[0]) {
            if (std::holds_alternative<double>(value.value()) ||
                std::holds_alternative<int>(value.value())) {
                numericCols.push_back(key);
                numericData.push_back(Utils::extractColumn<double>(data, key));
            }
        }

        std::filesystem::create_directories(std::filesystem::path(outputFile).parent_path());
        std::ofstream outFile(outputFile);
        outFile << std::fixed << std::setprecision(2);

        // Verifying all target columns exist and are numeric
        for (const auto& col : targetColumns) {
            if (std::find(numericCols.begin(), numericCols.end(), col) == numericCols.end()) {
                throw std::runtime_error("Column '" + col + "' does not exist or is not numeric");
            }
        }

        const int labelWidth = 20;
        const int valueWidth = 15;
        
        
        outFile << std::left << std::setw(labelWidth) << "Statistics";
        for (const auto& col : targetColumns) {
            outFile << "|" << std::setw(valueWidth) << col;
        }
        outFile << "\n";
        
        // Printing separator line
        outFile << std::string(labelWidth, '-');
        for (size_t i = 0; i < targetColumns.size(); ++i) {
            outFile << "+" << std::string(valueWidth-1, '-');
        }
        outFile << "\n";

        
        outFile << std::left << std::setw(labelWidth) << "Mean";
        for (const auto& col : targetColumns) {
            auto columnData = Utils::extractColumn<double>(data, col);
            outFile << "|" << std::setw(valueWidth) << Statistics::mean(columnData);
        }
        outFile << "\n";

        outFile << std::left << std::setw(labelWidth) << "Median";
        for (const auto& col : targetColumns) {
            auto columnData = Utils::extractColumn<double>(data, col);
            outFile << "|" << std::setw(valueWidth) << Statistics::median(columnData);
        }
        outFile << "\n";

        outFile << std::left << std::setw(labelWidth) << "Variance";
        for (const auto& col : targetColumns) {
            auto columnData = Utils::extractColumn<double>(data, col);
            outFile << "|" << std::setw(valueWidth) << Statistics::variance(columnData);
        }
        outFile << "\n";

        outFile << std::left << std::setw(labelWidth) << "Standard Deviation";
        for (const auto& col : targetColumns) {
            auto columnData = Utils::extractColumn<double>(data, col);
            outFile << "|" << std::setw(valueWidth) << Statistics::sd(columnData);
        }
        outFile << "\n\n";

        
        for (const auto& col : targetColumns) {
            auto columnData = Utils::extractColumn<double>(data, col);
            auto freqCount = Statistics::freqCount(columnData);
            outFile << "Frequency Count for " << col << ":\n";
            for (const auto& [key, value] : freqCount) {
                outFile << key << ": " << value << "\n";
            }
            outFile << "\n";
        }

        
        Eigen::MatrixXd dataMat(data.size(), numericCols.size());
        for (size_t i = 0; i < numericCols.size(); ++i) {
            std::vector<double> columnData = Utils::extractColumn<double>(data, numericCols[i]);
            for (size_t j = 0; j < data.size(); ++j) {
                dataMat(j, i) = columnData[j];
            }
        }

        Eigen::MatrixXd correlationMat = Statistics::correlationM(dataMat);
        Statistics::reportStrongCorr(correlationMat, numericCols, 0.7, outFile);

        std::cout << "Stat results saved to " << std::filesystem::current_path() / outputFile << std::endl;

    } catch (const std::exception& excep) {
        std::cerr << "Error: " << excep.what() << std::endl;
    }

    return 0;
}