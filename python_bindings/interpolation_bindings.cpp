#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // For std::vector conversion
#include "../include/Interpolation_Module/Interpolation_Module.hpp"

namespace py = pybind11;
using namespace ScientificToolbox::Interpolation;

PYBIND11_MODULE(_interpolation, m) {
    m.doc() = "Python bindings for the interpolation module";

    // Point class
    py::class_<point<double>>(m, "Point")
        .def(py::init<double, double>())
        .def("get_x", &point<double>::get_x)
        .def("get_y", &point<double>::get_y)
        .def("__repr__", [](const point<double>& p) {
            return "(" + std::to_string(p.get_x()) + ", " + std::to_string(p.get_y()) + ")";
        });

    // Function to convert pandas DataFrame to std::set<point<double>>
    m.def("df_to_set_of_points", [](py::object df) -> std::set<point<double>> {
        std::set<point<double>> points;
        auto x_values = df.attr("x").cast<std::vector<double>>();
        auto y_values = df.attr("y").cast<std::vector<double>>();

        if (x_values.size() != y_values.size()) {
            throw std::runtime_error("The x and y columns must have the same length.");
        }

        for (size_t i = 0; i < x_values.size(); ++i) {
            points.insert(point<double>(x_values[i], y_values[i]));
        }
        return points;
    });   

    // Interpolation base class
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
    
    // InterpolationTester class
    py::class_<InterpolationTester>(m, "InterpolationTester")
        .def(py::init())
        .def("run_tests", &InterpolationTester::run_tests);

    // AnalysisInterpolation class
    py::class_<AnalysisInterpolation>(m, "AnalysisInterpolation")
        .def(py::init())
        .def("AccuracyAnalysis", &AnalysisInterpolation::AccuracyAnalysis)
        .def("EfficiencyAnalysis", &AnalysisInterpolation::EfficiencyAnalysis)
        .def("OrderConvergenceAnalysis", &AnalysisInterpolation::OrderConvergenceAnalysis);
    
}
