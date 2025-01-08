#include <iostream>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <random>
#include "../include/Statistics_Module/Dataset.hpp"
#include "../include/Statistics_Module/Statistical_analyzer.hpp"

using namespace ScientificToolbox::Statistics;

class StatisticsModuleTest {
private:
    std::shared_ptr<Dataset> dataset;
    std::unique_ptr<StatisticalAnalyzer> analyzer;

    
    bool approx_equal(double a, double b, double epsilon = 1e-5) {
        return std::abs(a - b) < epsilon;
    }

public:
    void setUp() {
        
        std::vector<std::unordered_map<std::string, OptionalDataValue>> data = {
            {{"ColA", 1.0}, {"ColB", 10.0}},
            {{"ColA", 2.0}, {"ColB", 20.0}},
            {{"ColA", 3.0}, {"ColB", 30.0}},
            {{"ColA", 4.0}, {"ColB", 40.0}}
        };
        dataset = std::make_shared<Dataset>(data);
        analyzer = std::make_unique<StatisticalAnalyzer>(dataset);
    }

    void testMean() {
        double meanA = analyzer->mean<double>("ColA");
        double meanB = analyzer->mean<double>("ColB");
        assert(approx_equal(meanA, 2.5));
        assert(approx_equal(meanB, 25.0));
    }

    void testMedian() {
        double medianA = analyzer->median<double>("ColA");
        double medianB = analyzer->median<double>("ColB");
        assert(approx_equal(medianA, (2.0 + 3.0) / 2.0)); 
        assert(approx_equal(medianB, (20.0 + 30.0) / 2.0)); 
    }

    void testVariance() {
        double varA = analyzer->variance<double>("ColA");
        double varB = analyzer->variance<double>("ColB");
        assert(approx_equal(varA, 1.25));
        assert(approx_equal(varB, 125.0));
    }

    void testStdDev() {
        double stdA = analyzer->standardDeviation<double>("ColA");
        double stdB = analyzer->standardDeviation<double>("ColB");
        assert(approx_equal(stdA, 1.11803, 1e-3));
        assert(approx_equal(stdB, 11.1803, 1e-3));
    }

    void testCorrelation() {
        auto cm = analyzer->correlationMatrix({"ColA", "ColB"});
        assert(cm.rows() == 2 && cm.cols() == 2);
        assert(approx_equal(cm(0, 1), 1.0, 1e-1));
        assert(approx_equal(cm(1, 0), 1.0, 1e-1));
    }


    void TestNormal() {

        std::mt19937 gen(123);
        std::normal_distribution<double> dist(0.0, 1.0);

        std::vector<std::unordered_map<std::string, OptionalDataValue>> normalData;

        for (int i = 0; i < 1000; ++i) {
            normalData.push_back({{"Normal", dist(gen)}});
        }

        auto normalDataset = std::make_shared<Dataset>(normalData);
        auto normalAnalyzer = std::make_unique<StatisticalAnalyzer>(normalDataset);

        double computedMean = normalAnalyzer->mean<double>("Normal");
        double computedVariance = normalAnalyzer->variance<double>("Normal");

        assert(std::abs(computedMean) < 0.1);
        assert(std::abs(computedVariance - 1.0) < 0.2);





    }




    bool runAllTests() {
        try {
            setUp();
            testMean();
            testMedian();
            testVariance();
            testStdDev();
            testCorrelation();
            TestNormal();
        } catch (...) {
            return false;
        }
        return true;
    }
};

int main() {
    StatisticsModuleTest tester;
    if (tester.runAllTests()) {
        std::cout << "All tests passed!\n";
        return 0;
    } else {
        std::cerr << "Some tests failed!\n";
        return 1;
    }
}