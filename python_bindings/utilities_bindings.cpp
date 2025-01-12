#include "../include/Utilities.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <iostream>
#include <sstream>
#include <chrono>

namespace py = pybind11;
using namespace ScientificToolbox;

py::object measure_execution_time_py(py::function callback, py::args args, py::kwargs kwargs) {
    // Utilizza il template per chiamare la funzione Python
    return measure_execution_time<py::object>(
        [&callback](py::args args, py::kwargs kwargs) -> py::object {
            return callback(*args, **kwargs);
        },
        args, kwargs
    );
}

PYBIND11_MODULE(utilities, m) {
    m.doc() = "Modulo per misurare il tempo di esecuzione delle funzioni";

    m.def("timer_decorator", [](py::function callback) {
        return py::cpp_function([callback](py::args args, py::kwargs kwargs) {
            return measure_execution_time_py(callback, args, kwargs);
        });
    });
}