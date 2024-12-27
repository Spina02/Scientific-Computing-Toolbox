
#include <pybind11/pybind11.h>

#include "../include/Utilities/ImportCSV.hpp"
#include "../include/Utilities/ImportData.hpp"

namespace py = pybind11;

PYBIND11_MODULE(common_bindings, m) {
    m.doc() = "Python bindings for the common module";

    // ImportCSV class
    py::class_<ScientificToolbox::ImportCSV>(m, "ImportCSV")
        .def(py::init<>())
        .def("import", &ScientificToolbox::ImportCSV::import);

    
}