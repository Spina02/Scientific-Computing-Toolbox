#include "../include/Statistics_Module/Statistical_analyzer.hpp"
#include "../include/Utilities/ImportCSV.hpp"
#include <iostream>
#include <filesystem>

using namespace ScientificToolbox;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    try {
        std::string project_dir = std::filesystem::current_path().parent_path();
        std::string inputFile = project_dir + "/data/Food_and_Nutrition__.csv";
        std::string outputFile = project_dir + "/output/Statistics_output.txt";

        ImportCSV importer;
        importer.import(inputFile);
        auto rawData = importer.getData();

        // Create dataset
        auto dataset = std::make_shared<Statistics::Dataset>(rawData);
        
        // Create analyzer
        Statistics::StatisticalAnalyzer analyzer(dataset);

        // Create output file
        std::filesystem::create_directories(std::filesystem::path(outputFile).parent_path());
        std::ofstream outFile(outputFile);
        std::vector<std::string> columns = {"Weight", "Height"};

        // Generate statistics
        for (const auto& col : columns) {
            outFile << "Statistics for " << col << ":\n";
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
        }

        // Generate correlation analysis
        analyzer.reportStrongCorrelations(columns, 0.7, outFile);
        
        std::cout << "Statistics saved to: " << outputFile << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}