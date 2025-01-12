#include "../include/Statistics_Module/Statistical_analyzer.hpp"
#include "../include/Utilities.hpp"
#include <iostream>
#include <filesystem>

using namespace ScientificToolbox;

int main(int argc, char** argv) {
    try {
        std::string project_dir = std::filesystem::current_path();
        std::string inputFile = project_dir + "/data/Food_and_Nutrition__.csv";
        std::string outputFile = project_dir + "/output/Statistics_output.txt";

        // Fix argument handling logic
        if (argc == 2) {
            inputFile = project_dir + "/data/" + std::string(argv[1]);
        }
        else if (argc == 3) {
            inputFile = project_dir + "/data/" + std::string(argv[1]);
            outputFile = project_dir + "/output/" + std::string(argv[2]);
        }
        else if (argc > 3) {
            std::cout << "\nToo many arguments. Using only the first two arguments." << std::endl;
            inputFile = project_dir + "/data/" + std::string(argv[1]);
            outputFile = project_dir + "/output/" + std::string(argv[2]);
        }

        std::cout << "Using input file: " << inputFile << "\nOutput file: " << outputFile << std::endl;
        

        Importer importer;
        importer.import(inputFile);
        auto rawData = importer.getData();

        
        auto dataset = std::make_shared<Statistics::Dataset>(rawData);
        
        
        Statistics::StatisticalAnalyzer analyzer(dataset);

        std::filesystem::create_directories(std::filesystem::path(outputFile).parent_path());
        std::ofstream outFile(outputFile);
        std::vector<std::string> columns = {"Weight", "Height"};

        
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

        
        analyzer.reportStrongCorrelations(columns, 0.7, outFile);
        
        std::cout << "Statistics saved to: " << outputFile << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}