#include "../include/Statistics_Module/Statistical_analyzer.hpp"
#include "../include/Utilities.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>

using namespace ScientificToolbox;

int main() {
    try {
        std::string project_dir = std::filesystem::current_path();
        std::string inputFile = project_dir + "/data/Food_and_Nutrition__.csv";
        std::string outputFile = project_dir + "/output/Statistics_output.txt";

        std::cout << "Enter input filename from data folder (press Enter for default):\n";
        std::string userInput;
        std::getline(std::cin, userInput);
        if (!userInput.empty()) {
            inputFile = project_dir + "/data/" + userInput;
        }

        Importer importer;
        importer.import(inputFile);
        auto rawData = importer.getData();
        auto dataset = std::make_shared<Statistics::Dataset>(rawData);
        Statistics::StatisticalAnalyzer analyzer(dataset);

        std::vector<std::string> columns;
        std::cout << "Enter column names for analysis (comma-separated, press Enter for all numeric):\n";
        std::string columnInput;
        std::getline(std::cin, columnInput);

        if (columnInput.empty()) {
            auto allColumns = dataset->getColumnNames();
            for (const auto& col : allColumns) {
                if (dataset->isNumericColumn(col)) {
                    columns.push_back(col);
                }
            }
        } else {
            std::stringstream ss(columnInput);
            std::string col;
            while (std::getline(ss, col, ',')) {
                col.erase(0, col.find_first_not_of(" "));
                col.erase(col.find_last_not_of(" ") + 1);
                columns.push_back(col);
            }
        }

        std::filesystem::create_directories(std::filesystem::path(outputFile).parent_path());
        std::ofstream outFile(outputFile);

        for (const auto& col : columns) {
            outFile << "Statistics for " << col << ":\n";
            try {
                outFile << "Mean: " << analyzer.mean<double>(col) << "\n";
                outFile << "Median: " << analyzer.median<double>(col) << "\n";
                outFile << "Variance: " << analyzer.variance<double>(col) << "\n";
                outFile << "Standard Deviation: " << analyzer.standardDeviation<double>(col) << "\n\n";
                
                auto freqCount = analyzer.frequencyCount<double>(col);
                outFile << "Frequency distribution:\n";
                for (const auto& [value, count] : freqCount) {
                    outFile << value << ": " << count << "\n";
                }
                outFile << "\n";
            } catch (const std::exception& e) {
                outFile << "Could not analyze column: " << e.what() << "\n\n";
            }
        }

        analyzer.reportStrongCorrelations(columns, 0.7, outFile);
        
        std::cout << "Statistics saved to: " << outputFile << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}