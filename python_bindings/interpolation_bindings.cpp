
#include <pybind11/pybind11.h>

#include "../include/Utilities/ImportCSV.hpp"
#include "../include/Interpolation_Module/Interpolation.hpp"
#include "../include/Interpolation_Module/LinearInterpolation.hpp"
#include "../include/Interpolation_Module/Lagrange.hpp"
#include "../include/Interpolation_Module/Newton.hpp"
#include "../include/Interpolation_Module/Cubic_Spline_Interpolation.hpp"

#include "../include/Interpolation_Module/AnalysisInterpolation.hpp"
#include "../include/Interpolation_Module/InterpolationTester.hpp"

namespace py = pybind11;

PYBIND11_MODULE(interpolation_bindings, m) {
    m.doc() = "Python bindings for the interpolation module";

    // ImportCSV class
    py::class_<ScientificToolbox::ImportCSV>(m, "ImportCSV")
        .def(py::init<>())
        .def("import", &ScientificToolbox::ImportCSV::import);

    
}

