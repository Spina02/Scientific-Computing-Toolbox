#include "../../include/Statistics_Module/Dataset.hpp"
#include "../../include/Statistics_Module/Statistical_analyzer.hpp"


#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 
#include <pybind11/eigen.h>
#include <sstream>
namespace py = pybind11;

using namespace ScientificToolbox::Statistics;

PYBIND11_MODULE(_stats, m) {
    m.doc() = "Python bindings for the statistics module";

    py::class_<Dataset, std::shared_ptr<Dataset>>(m, "Dataset")
        .def(py::init<>())
        .def("addRow", &Dataset::addRow)
        .def("getColumn", &Dataset::getColumn<double>)
        .def("getColumn", &Dataset::getColumn<int>)
        .def("getColumn", &Dataset::getColumn<std::string>)
        .def("isNumeric", &Dataset::isNumericColumn)
        .def("getColumnNames", &Dataset::getColumnNames)  
        .def("size", &Dataset::size);

    py::class_<StatisticalAnalyzer>(m, "StatisticalAnalyzer")
        .def(py::init<std::shared_ptr<Dataset>>())
        .def("mean", &StatisticalAnalyzer::mean<double>)
        .def("median", &StatisticalAnalyzer::median<double>)
        .def("variance", &StatisticalAnalyzer::variance<double>)
        .def("standardDeviation", &StatisticalAnalyzer::standardDeviation<double>)
        .def("frequencyCount", &StatisticalAnalyzer::frequencyCount<double>)
        .def("frequencyCountStr", &StatisticalAnalyzer::frequencyCountStr<std::string>) 
        .def("correlationMatrix", &StatisticalAnalyzer::correlationMatrix)
        .def("reportStrongCorrelations", 
            [](StatisticalAnalyzer& self, const std::vector<std::string>& columnNames, double threshold) {
                std::stringstream ss;
                self.reportStrongCorrelations(columnNames, threshold, ss);
                return ss.str();
            },
            py::arg("columnNames"),
            py::arg("threshold") = 0.7);
}