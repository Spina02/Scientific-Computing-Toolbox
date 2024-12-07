#ifndef ODETESTER_HPP
#define ODETESTER_HPP

#include "../../include/Utilities.hpp"
#include "../../include/ODE_Module/ODESolver.hpp"
#include "../../include/ODE_Module/ExpressionParser.hpp"
#include "../../include/ODE_Module/FESolver.hpp"
#include <iostream>
#include <string>
#include <variant>
#include <vector>

namespace ScientificToolbox {

struct ScalarODETestCase {
    std::string expr;
    double t0;
    double tf;
    double h;
    double y0;
    double expected_final;
    double expected_derivative;
};

struct VectorODETestCase {
    vec_s exprs;
    double t0;
    double tf;
    double h;
    vec_d y0;
    vec_d expected_final;
    vec_d expected_derivative;
};

using test_case = std::variant<ScalarODETestCase, VectorODETestCase>;

class ODETester {
public:
    ODETester();
    ~ODETester() = default;

    bool test_expression_parser();
    bool test_FESolver();

private:
    // auxiliary functions
    bool test_scalar_expression(const std::string& expr, double t, double y, double expected, int test_num);
    bool test_vector_expression(const vec_s& exprs, double t, const vec_d& y, const vec_d& expected, int test_num);
    bool test_expression(const var_expr& expr_variant, double t_val, const var_vec& y_val, const var_vec& expected_val, int test_num);
    
    bool test_simple_ode(const test_case& case_variant, const std::string solver_type, int test_num);

    vec_d t;
    vec_d y;
    vec_s exprs;
    vec_d expected;

    vec_d t_vec;
    vec_d y_vec;
    std::vector<vec_s> exprs_vec;
    std::vector<vec_d> expected_vec;

    std::vector<ScalarODETestCase> scalar_cases;
    std::vector<VectorODETestCase> vector_cases;
};

}

#endif // ODETESTER_HPP