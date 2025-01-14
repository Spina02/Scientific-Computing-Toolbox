#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include "../../include/ODE_Module/ODE_Module.hpp"
#include "../../include/ODE_Module/ODETester.hpp"

namespace py = pybind11;
using namespace ScientificToolbox::ODE;

PYBIND11_MODULE(_ode, m) {
    m.doc() = R"pbdoc(
        ODE Solver Module
        ----------------

        A Python interface to the C++ ODE solver library.
        
        This module provides numerical solvers for Ordinary Differential Equations including:
        - Forward Euler method
        - Explicit Midpoint method
        - RK4 method (4th order Runge-Kutta)
        
        Key Functions:
            load_tests_from_csv - Load test cases from CSV file
            parse_var_expr - Parse variable expressions
            compute_error - Compute solution error
            solve_and_measure_execution_time - Measure solver performance
    )pbdoc";

    // Data importer:
    m.def("load_tests_from_csv", &load_tests_from_csv, py::arg("filename"),
        R"pbdoc(
        Load ODE test cases from a CSV file.

        Args:
            filename (str): Path to the CSV file containing test cases

        Returns:
            list: List of ODETestCase objects
        )pbdoc");

    // Utilities
    m.def("parse_var_expr", &parse_var_expr,
        R"pbdoc(
        Parse a string into a variable expression.

        Args:
            expr (str): String containing mathematical expression

        Returns:
            Expression object suitable for ODE solvers
        )pbdoc");

    m.def("parseExpression", &parseExpression,
        R"pbdoc(
        Convert a mathematical expression into a callable function.

        Args:
            expr: Mathematical expression of form f(t,y) or [f1(t,y), f2(t,y),...]

        Returns:
            Callable function object
        )pbdoc");

    m.def("get_solver_types", &get_solver_types);

    // Analysis utilities
    m.def("compute_error", &compute_error,
        R"pbdoc(
        Compute error between numerical and analytical solutions.

        Args:
            result: Numerical solution
            expected: Analytical solution

        Returns:
            float: Computed error value
        )pbdoc");

    m.def("solve_and_measure_execution_time", &solve_and_measure_execution_time,
        R"pbdoc(
        Measure execution time of ODE solver.

        Args:
            solver: ODESolver instance

        Returns:
            tuple: (ODESolution, execution_time)
        )pbdoc");

    m.def("compute_order_of_convergence", &compute_order_of_convergence,
        R"pbdoc(
        Compute the order of convergence for a given solver.

        Args:
            solver_type (str): Name of the solver type

        Returns:
            float: Computed order of convergence
        )pbdoc");

    m.def("save_to_csv", &save_to_csv, 
        py::arg("filename"), 
        py::arg("solution"), 
        py::arg("append") = false,
        R"pbdoc(
        Save ODE solution to CSV file.

        Args:
            filename (str): Output file path
            solution (ODESolution): Solution to save
            append (bool, optional): Append to existing file. Defaults to False.
        )pbdoc");

    // ODETestCase class
    py::class_<ODETestCase>(m, "ODETestCase")
        .def(py::init<var_expr, var_vec, double, double, double, std::optional<var_vec>, std::optional<var_vec>>(),
            py::arg("expr"),
            py::arg("y0"),
            py::arg("t0"),
            py::arg("tf"),
            py::arg("h"),
            py::arg("expected_solution") = std::nullopt,
            py::arg("expected_derivative") = std::nullopt,
            R"pbdoc(
            Test case for ODE solvers.

            Args:
                expr: ODE expression
                y0: Initial condition
                t0: Start time
                tf: End time
                h: Step size
                expected_solution: Optional analytical solution
                expected_derivative: Optional analytical derivative
            )pbdoc")
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
    py::class_<ODESolution>(m, "ODESolution", R"pbdoc(
        Stores the solution of an ODE system.

        Attributes:
            size (int): Dimension of the system
            t_values (array): Time points
            y_values (array): Solution values at each time point
        )pbdoc")
        .def("get_solution", &ODESolution::get_solution, "Get complete solution array")
        .def("get_result", &ODESolution::get_result, "Get final solution values")
        .def("get_times", &ODESolution::get_times, "Get time points array")
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
    py::class_<ODESolver>(m, "ODESolver", R"pbdoc(
        Base class for ODE solvers.
        )pbdoc")
        .def("solve", &ODESolver::solve, "Solve the ODE system");

    // ForwardEulerSolver class
    py::class_<ForwardEulerSolver, ODESolver>(m, "ForwardEulerSolver", R"pbdoc(
        Forward Euler method solver.

        First-order numerical method for solving ODEs.
        )pbdoc")
        .def(py::init<var_expr&, var_vec&, double, double, double>(),
            py::arg("expr"),
            py::arg("y0"),
            py::arg("t0"),
            py::arg("tf"),
            py::arg("h"),
            R"pbdoc(
            Initialize Forward Euler solver.

            Args:
                expr: ODE expression
                y0: Initial condition
                t0: Start time
                tf: End time
                h: Step size
            )pbdoc")
        .def(py::init<ODETestCase>(), py::arg("test_case"));

    // ExplicitMidpointSolver class
    py::class_<ExplicitMidpointSolver, ODESolver>(m, "ExplicitMidpointSolver", R"pbdoc(
        Explicit Midpoint method solver.

        Second-order Runge-Kutta method for solving ODEs.
        )pbdoc")
        .def(py::init<var_expr&, var_vec&, double, double, double>(),
            py::arg("expr"),
            py::arg("y0"),
            py::arg("t0"),
            py::arg("tf"),
            py::arg("h"),
            R"pbdoc(
            Initialize Explicit Midpoint solver.

            Args:
                expr: ODE expression
                y0: Initial condition
                t0: Start time
                tf: End time
                h: Step size
            )pbdoc")
        .def(py::init<ODETestCase>(), py::arg("test_case"));

    // RK4Solver class
    py::class_<RK4Solver, ODESolver>(m, "RK4Solver", R"pbdoc(
        4th order Runge-Kutta method solver.

        Classical RK4 method providing high accuracy solution for ODEs.
        )pbdoc")
        .def(py::init<var_expr&, var_vec&, double, double, double>(),
            py::arg("expr"),
            py::arg("y0"),
            py::arg("t0"),
            py::arg("tf"),
            py::arg("h"),
            R"pbdoc(
            Initialize RK4 solver.

            Args:
                expr: ODE expression
                y0: Initial condition
                t0: Start time
                tf: End time
                h: Step size
            )pbdoc")
        .def(py::init<ODETestCase>(), py::arg("test_case"));

    py::class_<ODETester>(m, "ODETester", R"pbdoc(
        Testing framework for ODE solvers.

        Provides functionality to test expression parser and ODE solvers.
        )pbdoc")
        .def(py::init<>())
        .def(py::init<std::string>(), py::arg("filename"))
        .def("run_parser_tests", &ODETester::run_parser_tests, "Run expression parser tests")
        .def("run_ode_tests", &ODETester::run_ode_tests, "Run ODE solver tests");
}
