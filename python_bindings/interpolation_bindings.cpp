
#include <pybind11/pybind11.h>

#include "../include/Interpolation_Module/Interpolation.hpp"
#include "../include/Interpolation_Module/LinearInterpolation.hpp"
#include "../include/Interpolation_Module/Lagrange.hpp"
#include "../include/Interpolation_Module/Newton.hpp"
#include "../include/Interpolation_Module/Cubic_Spline_Interpolation.hpp"

#include "../include/Interpolation_Module/AnalysisInterpolation.hpp"
#include "../include/Interpolation_Module/InterpolationTester.hpp"

namespace py = pybind11;
using namespace ScientificToolbox::Interpolation;

PYBIND11_MODULE(interpolation_bindings, m) {
    m.doc() = "Python bindings for the interpolation module";

    // Interpolation class
    py::class_<Interpolation<double>>(m, "Interpolation")
        .def("interpolate", &Interpolation<double>::interpolate)
        .def("__call__", &Interpolation<double>::operator());

    // LinearInterpolation class
    py::class_<LinearInterpolation<double>, Interpolation<double>>(m, "LinearInterpolation")
        .def(py::init<const std::set<point<double>>&>())
        .def("interpolate", &LinearInterpolation<double>::interpolate);

    // Lagrange class
    py::class_<Lagrange<double>, Interpolation<double>>(m, "Lagrange")
        .def(py::init<const std::set<point<double>>&>())
        .def("interpolate", &Lagrange<double>::interpolate);

    // Newton class
    py::class_<Newton<double>, Interpolation<double>>(m, "Newton")
        .def(py::init<const std::set<point<double>>&>())
        .def("interpolate", &Newton<double>::interpolate);

    // SplineInterpolation class
    py::class_<SplineInterpolation<double>, Interpolation<double>>(m, "SplineInterpolation")
        .def(py::init<const std::set<point<double>>&>())
        .def("interpolate", &SplineInterpolation<double>::interpolate);    
}

