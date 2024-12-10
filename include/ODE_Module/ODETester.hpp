#ifndef ODETESTER_HPP
#define ODETESTER_HPP

#include "ODESolver.hpp"
#include "ExpressionParser.hpp"
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

    bool test_expression_parser();
    bool run_tests();

private:
    // auxiliary functions
    bool test_expression(const var_expr& expr_variant, double t_val, const var_vec& y_val, const var_vec& expected_val, int test_num);
    bool test_simple_ode(const ODETestCase& test_case, const std::string solver_type, int test_num);
    

    vec_d t;
    vec_d y;
    vec_s exprs;
    vec_d expected;

    vec_d t_vec;
    vec_d y_vec;
    std::vector<vec_s> exprs_vec;
    std::vector<vec_d> expected_vec;
};

}

#endif // ODETESTER_HPP