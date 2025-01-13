#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include "../include/ODE_Module.hpp"

namespace py = pybind11;
using namespace ScientificToolbox::ODE;

PYBIND11_MODULE(_ode, m) {
    m.doc() = "Python bindings for the ODE module";

    // Data importer:
    m.def("load_tests_from_csv", &load_tests_from_csv, py::arg("filename"));

    m.def("save_to_csv", &save_to_csv, 
        py::arg("filename"), 
        py::arg("solution"), 
        py::arg("append") = false);

    // ODETestCase class
    py::class_<ODETestCase>(m, "ODETestCase")
        .def_readwrite("expr", &ODETestCase::expr)
        .def_readwrite("t0", &ODETestCase::t0)
        .def_readwrite("tf", &ODETestCase::tf)
        .def_readwrite("h", &ODETestCase::h)
        .def_readwrite("y0", &ODETestCase::y0)
        .def_readwrite("expected_solution", &ODETestCase::expected_solution)
        .def_readwrite("expected_derivative", &ODETestCase::expected_derivative)
        .def("get_expr", &ODETestCase::get_expr)
        .def("has_expected_solution", &ODETestCase::has_expected_solution)
        .def("get_expected_solution", &ODETestCase::get_expected_solution)
        .def("__str__", [](const ODETestCase& tc) {
            std::ostringstream ss;
            ss << tc;
            return ss.str();
        });

    // ODESolution class
    py::class_<ODESolution>(m, "ODESolution")
        .def("get_solution", &ODESolution::get_solution)
        .def("get_result", &ODESolution::get_result)
        .def("get_times", &ODESolution::get_times)
        .def("get_size", &ODESolution::get_size)
        .def("get_expr", &ODESolution::get_expr)
        .def("get_initial_conditions", &ODESolution::get_initial_conditions)
        .def("get_final_time", &ODESolution::get_final_time)
        .def("get_step_size", &ODESolution::get_step_size)
        .def("__str__", [](const ODESolution& sol) {
            std::ostringstream ss;
            ss << sol;
            return ss.str();
        });

    // ODESolver class
    py::class_<ODESolver>(m, "ODESolver")
        .def("solve", &ODESolver::solve);

    // ForwardEulerSolver class
    py::class_<ForwardEulerSolver, ODESolver>(m, "ForwardEulerSolver")
        .def(py::init<var_expr&, var_vec&, double, double, double>())
        .def(py::init<ODETestCase>());

    // ExplicitMidpointSolver class
    py::class_<ExplicitMidpointSolver, ODESolver>(m, "ExplicitMidpointSolver")
        .def(py::init<var_expr&, var_vec&, double, double, double>())
        .def(py::init<ODETestCase>());

    // RK4Solver class
    py::class_<RK4Solver, ODESolver>(m, "RK4Solver")
        .def(py::init<var_expr&, var_vec&, double, double, double>())
        .def(py::init<ODETestCase>());

    // Analysis utilities
    m.def("compute_error", &compute_error);
    m.def("solve_and_measure_execution_time", &solve_and_measure_execution_time);
    m.def("compute_order_of_convergence", &compute_order_of_convergence);
    m.def("get_solver_types", &get_solver_types);
    m.def("parseExpression", &parseExpression);
    m.def("parse_var_expr", &parse_var_expr);
}
