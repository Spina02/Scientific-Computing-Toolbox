#ifndef ODETESTER_HPP
#define ODETESTER_HPP

#include "ODESolver.hpp"
#include "ForwardEulerSolver.hpp"
#include "RK4Solver.hpp"
#include "ExplicitMidpointSolver.hpp"
#include "../Utilities/ImportCSV.hpp"
#include <iostream>

namespace ScientificToolbox::ODE {

static inline const vec_s solver_types = {"ForwardEulerSolver", "RK4Solver", "ExplicitMidpointSolver"}; 

class ODETester {
public:
    ODETester();
    ~ODETester() = default;

    bool run_parser_tests() const;
    bool run_ode_tests() const;

private:
    // auxiliary functions
    bool test_expression(const var_expr& expr_variant, double t_val, const var_vec& y_val, const var_vec& expected_val, int test_num) const;
    bool test_simple_ode(const ODETestCase& test_case, const std::string solver_type, int test_num) const;


};

}

#endif // ODETESTER_HPP