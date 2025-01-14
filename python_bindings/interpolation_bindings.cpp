/**
 * \file interpolation_bindings.cpp
 * \brief Python bindings for the interpolation module.
 *
 * Exposes the Point class and a helper function df_to_set_of_points
 * that converts a pandas DataFrame into a set of points.
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // For std::vector conversion
#include "../include/Interpolation_Module/Interpolation_Module.hpp"

namespace py = pybind11;
using namespace ScientificToolbox::Interpolation;

/**
 * \brief Defines the `_interpolation` Python module.
 * \param m The module instance to which we add bindings.
 */
PYBIND11_MODULE(_interpolation, m) {
    m.doc() = "Python bindings for the interpolation module";

    // Point class
    /**
     * \class point<double>
     * \brief Represents a 2D point with double precision coordinates.
     */
    py::class_<point<double>>(m, "Point")
        .def(py::init<double, double>())
        .def("get_x", &point<double>::get_x)
        .def("get_y", &point<double>::get_y)
        .def("__repr__", [](const point<double>& p) {
            return "(" + std::to_string(p.get_x()) + ", " + std::to_string(p.get_y()) + ")";
        });

    // Function to convert pandas DataFrame to std::set<point<double>>
    /**
     * \brief Converts a pandas DataFrame with 'x' and 'y' columns to a set of points.
     * \param df A pandas DataFrame object with 'x' and 'y' columns.
     * \return A set of point<double> instances constructed from the DataFrame values.
     */
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
    }, "Convert a pandas DataFrame with 'x' and 'y' columns to a set of points.");   

    // Interpolation base class
    /**
     * \class Interpolation<double>
     * \brief Abstract base class for interpolation approaches.
     */
    py::class_<Interpolation<double>>(m, "Interpolation")
        .def("interpolate", &Interpolation<double>::interpolate)
        .def("__call__", &Interpolation<double>::operator());

    // LinearInterpolation class
    /**
     * \class LinearInterpolation<double>
     * \brief Implements linear interpolation over a set of points.
     */
    py::class_<LinearInterpolation<double>, Interpolation<double>>(m, "LinearInterpolation")
        .def(py::init<const std::set<point<double>>&>())
        .def("interpolate", &LinearInterpolation<double>::interpolate,
             "Interpolate the value at a given x using linear interpolation.");

    // Lagrange class
    /**
     * \class Lagrange<double>
     * \brief Implements Lagrange polynomial interpolation over a set of points.
     */
    py::class_<Lagrange<double>, Interpolation<double>>(m, "Lagrange")
        .def(py::init<const std::set<point<double>>&>())
        .def("interpolate", &Lagrange<double>::interpolate,
             "Interpolate the value at a given x using Lagrange polynomial interpolation.");

    // Newton class
    /**
     * \class Newton<double>
     * \brief Implements Newton polynomial interpolation using divided differences.
     */
    py::class_<Newton<double>, Interpolation<double>>(m, "Newton")
        .def(py::init<const std::set<point<double>>&>())
        .def("interpolate", &Newton<double>::interpolate,
             "Interpolate the value at a given x using Newton polynomial interpolation.");

    // SplineInterpolation class
    /**
     * \class SplineInterpolation<double>
     * \brief Implements spline interpolation for a set of points.
     */
    py::class_<SplineInterpolation<double>, Interpolation<double>>(m, "SplineInterpolation")
        .def(py::init<const std::set<point<double>>&>())
        .def("interpolate", &SplineInterpolation<double>::interpolate,
             "Interpolate the value at a given x using spline interpolation.");    
    
    // InterpolationTester class
    /**
     * \class InterpolationTester
     * \brief Provides test functionality and example usage of interpolation classes.
     */
    py::class_<InterpolationTester>(m, "InterpolationTester")
        .def(py::init())
        .def("run_tests", &InterpolationTester::run_tests,
             "Run tests for the interpolation classes.");

    // AnalysisInterpolation class
    /**
     * \class AnalysisInterpolation
     * \brief Provides analysis methods for interpolation, including accuracy, efficiency, and order of convergence.
     */
    py::class_<AnalysisInterpolation>(m, "AnalysisInterpolation")
        .def(py::init(), "Construct a default AnalysisInterpolation object.")
        .def("AccuracyAnalysis", &AnalysisInterpolation::AccuracyAnalysis,
             "Analyze the accuracy of interpolation.")
        .def("EfficiencyAnalysis", &AnalysisInterpolation::EfficiencyAnalysis,
             "Analyze the efficiency of interpolation.")
        .def("OrderConvergenceAnalysis", &AnalysisInterpolation::OrderConvergenceAnalysis,
             "Analyze the order of convergence of interpolation.");
    
}
