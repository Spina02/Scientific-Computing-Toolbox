#include "../include/Utilities.hpp"
#include "../include/ODE_Module/ODESolver.hpp"
#include "../include/ODE_Module/ExpressionParser.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace ScientificToolbox;

bool test_scalar_expression(const std::string& expr, double t, double y, double expected, int test_num) {
    auto scalar_f = parseScalarExpression(expr);
    double result = scalar_f(t, y);
    if (std::abs(result - expected) > 1e-4) {
        std::cout << "Test " << test_num << " failed: Scalar function value is incorrect for expression: " << expr << std::endl;
        return false;
    }
    std::cout << "Test " << test_num << " passed" << std::endl;
    return true;
}

bool test_vector_expression(const vec_s& exprs, double t, const vec_d& y, const vec_d& expected, int test_num) {
    auto vector_f = parseVectorExpression(exprs);
    auto results = vector_f(t, y);
    for (size_t i = 0; i < results.size(); ++i) {
        if (std::abs(results[i] - expected[i]) > 1e-5) {
            std::cout << "Test " << test_num << " failed: Vector function value is incorrect for expression: " << exprs[i] << std::endl;
            return false;
        }
    }
    std::cout << "Test " << test_num << " passed" << std::endl;
    return true;
}

bool test_expression_parser() {
    std::vector<bool> res;
    int test_counter = 1;

    vec_s exprs = {"y * t",
         "y + t * t",
         "y * sin(t)",
         "y * cos(t)"};

    vec_d t = {2.0, 2.0, 1.5708, 3.14159};

    vec_d y = {3.0, 3.0, 2.0, 2.0};

    vec_d expected = {6.0, 7.0, 2.0, -2.0};

    for (size_t i = 0; i < exprs.size(); ++i) {
        res.push_back(test_scalar_expression(exprs[i], t[i], y[i], expected[i], test_counter++));
    }

    std::vector<vec_s> exprs_vec = {
        {"y0 + t", "y1 + t * t", "y2 - t"},
        {"y0 - t", "y1 * y1", "y2 / t"},
        {"t / y0", "t / y1", "t / y2"},
        {"y0 * y2 - y1", "y1 * y2 - t", "y0 + y1 + y2"}
    };

    std::vector<vec_d> expected_vec = {
        {3.0, 6.0, 2.0},
        {-1.0, 4.0, 2.0},
        {2.0, 1.0, 0.5},
        {2.0, 6.0, 7.0}
    };

    y = {1.0, 2.0, 4.0};

    vec_d t_vec = {2.0, 2.0, 2.0, 2.0};

    for (size_t i = 0; i < exprs_vec.size(); ++i) {
        res.push_back(test_vector_expression(exprs_vec[i], t_vec[i], y, expected_vec[i], test_counter++));
    }

    int total_tests = res.size();
    int passed_tests = std::count(res.begin(), res.end(), true);
    
    std::cout << "\nSummary: " << passed_tests << "/" << total_tests << " tests passed." << std::endl;
    
    return std::all_of(res.begin(), res.end(), [](bool v) { return v; });
}

int main() {

    if (test_expression_parser()) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cerr << "Some tests failed!" << std::endl;
    }

    return 0;
}