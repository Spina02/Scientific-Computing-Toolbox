#include "../../include/Statistics_Module/Dataset.hpp"
#include "../../include/Statistics_Module/Statistical_analyzer.hpp"


#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 
#include <pybind11/eigen.h>
#include <sstream>
namespace py = pybind11;

using namespace ScientificToolbox::Statistics;


PYBIND11_MODULE(_stats, m) {
    m.doc() = R"pbdoc(
                        Python bindings for the statistics module providing Dataset and StatisticalAnalyzer functionalities.)pbdoc";

    py::class_<Dataset, std::shared_ptr<Dataset>>(m, "Dataset", R"pbdoc(
                        Represents a dataset for statistical analysis.)pbdoc")
        .def(py::init<>(), R"pbdoc(
                        Creates an empty Dataset.)pbdoc")
        .def("addRow", &Dataset::addRow, R"pbdoc(
                        Adds a new row to the Dataset. All columns must match existing structure.)pbdoc")
        .def("getColumn", &Dataset::getColumn<double>, R"pbdoc(
                        Gets the specified column as a vector of doubles.)pbdoc")
        .def("getColumn", &Dataset::getColumn<int>, R"pbdoc(
                        Gets the specified column as a vector of ints.)pbdoc")
        .def("getColumn", &Dataset::getColumn<std::string>, R"pbdoc(
                        Gets the specified column as a vector of strings.)pbdoc")
        .def("isNumeric", &Dataset::isNumericColumn, R"pbdoc(
                        Checks if the specified column contains only numeric data.)pbdoc")
        .def("getColumnNames", &Dataset::getColumnNames, R"pbdoc(
                        Returns the names of all columns in the Dataset.)pbdoc")
        .def("size", &Dataset::size, R"pbdoc(
                        Returns the number of rows in the Dataset.)pbdoc");

    py::class_<StatisticalAnalyzer>(m, "StatisticalAnalyzer", R"pbdoc(
                        Performs statistical computations on a Dataset.)pbdoc")
        .def(py::init<std::shared_ptr<Dataset>>(), R"pbdoc(
                        Constructs a StatisticalAnalyzer with the given Dataset.)pbdoc")
        .def("mean", &StatisticalAnalyzer::mean<double>, R"pbdoc(
                        Computes the mean of the specified column.)pbdoc")
        .def("median", &StatisticalAnalyzer::median<double>, R"pbdoc(
                        Computes the median of the specified column.)pbdoc")
        .def("variance", &StatisticalAnalyzer::variance<double>, R"pbdoc(
                        Computes the variance of the specified column.)pbdoc")
        .def("standardDeviation", &StatisticalAnalyzer::standardDeviation<double>, R"pbdoc(
                        Computes the standard deviation of the specified column.)pbdoc")
        .def("frequencyCount", &StatisticalAnalyzer::frequencyCount<double>, R"pbdoc(
                        Calculates the frequency distribution for numeric columns.)pbdoc")
        .def("frequencyCountStr", &StatisticalAnalyzer::frequencyCount<std::string>, R"pbdoc(
                        Calculates the frequency distribution for string columns.)pbdoc")
        .def("correlationMatrix", &StatisticalAnalyzer::correlationMatrix, R"pbdoc(
                        Generates a correlation matrix for the specified columns.)pbdoc")
        .def("reportStrongCorrelations",
             [](StatisticalAnalyzer& self, const std::vector<std::string>& columnNames, double threshold) {
                 std::stringstream ss;
                 self.reportStrongCorrelations(columnNames, threshold, ss);
                 return ss.str();
             },
             py::arg("columnNames"),
             py::arg("threshold") = 0.7,
             R"pbdoc(
                        Reports columns with absolute correlation above the given threshold.)pbdoc");
}
