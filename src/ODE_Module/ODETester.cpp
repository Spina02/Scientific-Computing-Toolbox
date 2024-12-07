#include "../../include/ODE_Module/ODETester.hpp"
#include <cmath>
#include <functional>
#include <muParser.h>
#include <memory>
#include <algorithm>
#include <iostream>

namespace ScientificToolbox {

ODETester::ODETester() {

    // Scalar tests initialization

    scalar_cases.push_back({
        "y * t",     // dy/dt = y * t
        0.0,         // t0
        1.0,         // tf
        0.01,         // h
        1.0,         // y0
        std::exp(0.5 * 1.0 * 1.0), // Exact solution: y(t) = y0 * e^(0.5 * t^2)
        0          // Exact derivative: y'(t) = 0.1 * 1 = 0.1
    });

    scalar_cases.push_back({
        "-y",        // dy/dt = -y
        0.0,         // t0
        1.0,         // tf
        0.01,         // h
        1.0,         // y0
        std::exp(-1.0), // Exact solution: y(t) = y0 * e^(-t)
        -1         // Exact derivative: y'(t) = -1
    });

    // Vector tests initialization

    // Example 3: System of exponential ODEs
    // dy0/dt = y0 + y1
    // dy1/dt = y0 - y1

    vector_cases.push_back({
        {"y0", "-y1"},             // Expressions
        0.0,                                // t0
        1.0,                                // tf
        0.01,                                // h
        (vec_d(2) << 1.0, 2.0).finished(),  // y0
        // Exact solutions: y0(t) = e^t, y1(t) = 0
        (vec_d(2) << std::exp(1.0), 2 * std::exp(-1.0)).finished(), // Expected values at tf
        (vec_d(2) << 1.0, -2.0).finished() // Expected value of f(y0, t0)
    });

    // Example 4: System of trigonometric ODEs
    // dy0/dt = y0 * sin(t)
    // dy1/dt = y1 * cos(t)

    vector_cases.push_back({
        {"2 * y0 + y1", "2 * y1"},     // Expressions
        0.0,                                // t0
        1.0,                                // tf
        0.001,                                // h
        (vec_d(2) << 1.0, 2.0).finished(),  // y0
        // Exact solutions: y0(t) = 3 * e^t, y1(t) = 2 * e^2t
        (vec_d(2) << (1.0 + 2.0) * std::exp(2.0), 2.0 * std::exp(2.0)).finished(),
        // Expected value of f(y0, t0)
        (vec_d(2) << 4.0, 4.0).finished()
    });
}

bool ODETester::test_scalar_expression(const std::string& expr, double t_val, double y_val, double expected_val, int test_num) {
    try {
        auto scalar_f = parseScalarExpression(expr);
        double result = scalar_f(t_val, y_val);
        if (std::abs(result - expected_val) > 1e-4) {
            std::cout << "Test " << test_num << " failed: Scalar function value is incorrect for expression: " << expr << std::endl;
            std::cout << "Expected: " << expected_val << ", Got: " << result << std::endl;
            return false;
        }
        std::cout << "Test " << test_num << " passed" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "Test " << test_num << " failed with exception: " << e.what() << std::endl;
        return false;
    }
}

bool ODETester::test_vector_expression(const vec_s& exprs, double t_val, const vec_d& y_val, const vec_d& expected_val, int test_num) {
    try {
        auto vector_f = parseVectorExpression(exprs);
        auto results = vector_f(t_val, y_val);
        for (Eigen::Index i = 0; i < results.size(); ++i) {
            if (std::abs(results[i] - expected_val[i]) > 1e-5) {
                std::cout << "Test " << test_num << " failed: Vector function value is incorrect for expression: " << exprs[i] << std::endl;
                std::cout << "Expected: " << expected_val.transpose() << ", Got: " << results.transpose() << std::endl;
                return false;
            }
        }
        std::cout << "Test " << test_num << " passed" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "Test " << test_num << " failed with exception: " << e.what() << std::endl;
        return false;
    }
}

bool ODETester::test_expression(const var_expr& expr_variant, double t_val, const var_vec& y_val, const var_vec& expected_val, int test_num) {
    // Check if the expression is scalar or vectorial
    if (std::holds_alternative<std::string>(expr_variant)) {
        // Scalar expression
        const std::string& expr = std::get<std::string>(expr_variant);
        
        // Check if y_val and expected_val are doubles
        if (!std::holds_alternative<double>(y_val) || !std::holds_alternative<double>(expected_val)) {
            std::cout << "Test " << test_num << " failed: For scalar expression, y_val and expected_val must be double." << std::endl;
            return false;
        }

        double y = std::get<double>(y_val);
        double exp_val = std::get<double>(expected_val);

        return test_scalar_expression(expr, t_val, y, exp_val, test_num);

    } else if (std::holds_alternative<vec_s>(expr_variant)) {
        // Vector expression
        const vec_s& exprs = std::get<vec_s>(expr_variant);

        // Check if y_val and expected_val are VectorXd
        if (!std::holds_alternative<vec_d>(y_val) || !std::holds_alternative<vec_d>(expected_val)) {
            std::cout << "Test " << test_num << " failed: For vector expression, y_val and expected_val must be VectorXd." << std::endl;
            return false;
        }

        const vec_d& y = std::get<vec_d>(y_val);
        const vec_d& exp_val = std::get<vec_d>(expected_val);

        return test_vector_expression(exprs, t_val, y, exp_val, test_num);

    } else {
        std::cout << "Test " << test_num << " failed: Unknown expression type." << std::endl;
        return false;
    }
}

bool ODETester::test_simple_ode(const test_case& case_variant, const std::string solver_type, int test_num) {
    try {
        std::cout << "\n=== Test " << test_num << " ===" << std::endl;

        // Extract parameters from appropriate test case
        var_expr expr_variant;
        double t0, tf, h;
        var_vec y0, expected_final;

        if (const auto* scalar_case = std::get_if<ScalarODETestCase>(&case_variant)) {
            expr_variant = scalar_case->expr;
            t0 = scalar_case->t0;
            tf = scalar_case->tf;
            h = scalar_case->h;
            y0 = scalar_case->y0;
            expected_final = scalar_case->expected_final;
        } 
        else if (const auto* vector_case = std::get_if<VectorODETestCase>(&case_variant)) {
            expr_variant = vector_case->exprs;
            t0 = vector_case->t0;
            tf = vector_case->tf;
            h = vector_case->h;
            y0 = vector_case->y0;
            expected_final = vector_case->expected_final;
        }

        // Print ODE info
        if (std::holds_alternative<std::string>(expr_variant)) {
            std::cout << "ODE: " << std::get<std::string>(expr_variant) << std::endl;
        } else {
            std::cout << "ODE system: " << std::endl;
            for (const auto& e : std::get<vec_s>(expr_variant)) {
                std::cout << e << std::endl;
            }
        }

        std::cout << "t0 = " << t0 << ", tf = " << tf << ", h = " << h << std::endl;

        // Print initial conditions
        std::cout << "Initial condition: ";
        if (auto* scalar_y0 = std::get_if<double>(&y0)) {
            std::cout << *scalar_y0 << std::endl;
        } else if (auto* vector_y0 = std::get_if<vec_d>(&y0)) {
            std::cout << vector_y0->transpose() << std::endl;
        }

        var_func f;

        // Parse expression based on type
        if (std::holds_alternative<std::string>(expr_variant)) {
            // Scalar case
            const std::string& expr = std::get<std::string>(expr_variant);
            f = parseScalarExpression(expr);
        } else {
            // Vector case
            const vec_s& exprs = std::get<vec_s>(expr_variant);
            f = parseVectorExpression(exprs);
        }

        std::unique_ptr<ODESolver> solver = nullptr;
        double sensitivity = 0.0;

        if (solver_type == "ForwardEulerSolver") {
            solver = std::make_unique<ForwardEulerSolver>(f, t0, y0, tf, h);
            sensitivity = 0.1;
        } else {
            std::cout << "Test " << test_num << " failed: Unknown solver type." << std::endl;
            return false;
        }

        auto results = solver->Solve();
        if (results.empty()) {
            std::cout << "Test " << test_num << " failed: No results produced" << std::endl;
            return false;
        }

        // Print trajectory
        if (DEBUG) {
            std::cout << "\nSolution trajectory:" << std::endl;
            double t_cur = t0;
            for (const auto& result : results) {
                std::cout << "t = " << t_cur << ", y = ";
                if (auto* scalar_res = std::get_if<double>(&result)) {
                    std::cout << *scalar_res;
                } else if (auto* vector_res = std::get_if<vec_d>(&result)) {
                    std::cout << vector_res->transpose();
                }
                std::cout << std::endl;
                t_cur += h;
            }
        }

        // Final comparison
        const var_vec& final_value = results.back();
        std::cout << "\nComparing final values:" << std::endl;

        // Expected
        std::cout << "Expected: ";
        if (auto* scalar_exp = std::get_if<double>(&expected_final)) {
            std::cout << *scalar_exp << std::endl;
        } else if (auto* vector_exp = std::get_if<vec_d>(&expected_final)) {
            std::cout << vector_exp->transpose() << std::endl;
        }

        // Got
        std::cout << "Got: ";
        if (auto* scalar_result = std::get_if<double>(&final_value)) {
            std::cout << *scalar_result << std::endl;
            if (auto* scalar_exp = std::get_if<double>(&expected_final)) {
                double error = std::abs(*scalar_result - *scalar_exp);
                if (error > sensitivity) {
                    std::cout << "Test " << test_num << " failed: error = " << error << std::endl;
                    return false;
                }
            } else {
                std::cout << "Test " << test_num << " failed: type mismatch between expected and got." << std::endl;
                return false;
            }
        } else if (auto* vector_result = std::get_if<vec_d>(&final_value)) {
            if (auto* vector_exp = std::get_if<vec_d>(&expected_final)) {
                std::cout << vector_result->transpose() << std::endl;
                if (vector_result->size() != vector_exp->size()) {
                    std::cout << "Test " << test_num << " failed: Vector size mismatch" << std::endl;
                    return false;
                }
                for (Eigen::Index i = 0; i < vector_result->size(); ++i) {
                    double error = std::abs((*vector_result)[i] - (*vector_exp)[i]);
                    if (error > sensitivity) {
                        std::cout << "Test " << test_num << " failed: Vector value mismatch at index " << i << ". Error = " << error << std::endl;
                        return false;
                    }
                }
            } else {
                std::cout << "Test " << test_num << " failed: type mismatch between expected and got." << std::endl;
                return false;
            }
        }

        std::cout << "Test " << test_num << " passed" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cout << "Test " << test_num << " failed with exception: " << e.what() << std::endl;
        return false;
    }
}

bool ODETester::test_expression_parser() {
    std::vector<bool> res;
    int test_counter = 1;

    std::cout << "\nStarting Expression Parser Tests\n" << std::endl;

    // Test scalar expressions
    for (const auto& test : scalar_cases) {
        res.push_back(test_expression(test.expr, 
                                    test.t0, 
                                    test.y0, 
                                    test.expected_derivative, 
                                    test_counter++));
    }

    // Test vector expressions
    for (const auto& test : vector_cases) {
        res.push_back(test_expression(test.exprs,
                                    test.t0,
                                    test.y0,
                                    test.expected_derivative,
                                    test_counter++));
    }

    // Print summary
    int total_tests = static_cast<int>(res.size());
    int passed_tests = std::count(res.begin(), res.end(), true);
    
    std::cout << "\nTest Summary: " << passed_tests << "/" << total_tests 
              << " tests passed." << std::endl;
    
    bool all_passed = std::all_of(res.begin(), res.end(), [](bool v) { return v; });
    if (all_passed) {
        std::cout << "All tests passed!\n" << std::endl;
    } else {
        std::cout << "Some tests failed!\n" << std::endl;
    }
    
    return all_passed;
}

bool ODETester::test_FESolver() {
    std::cout << "\nStarting Forward Euler Solver Tests\n" << std::endl;
    std::vector<bool> results;
    int test_counter = 1;

    // Scalar tests
    for (const auto& test : scalar_cases) {
        var_vec y0_var = test.y0;
        var_vec expected_var = test.expected_final;
        results.push_back(test_simple_ode(test, "ForwardEulerSolver", test_counter++));
    }

    // Vector tests
    for (const auto& test : vector_cases) {
        try {
            var_vec y0_var = test.y0;
            var_vec expected_var = test.expected_final;
            results.push_back(test_simple_ode(test, "ForwardEulerSolver", test_counter++));
        } catch (const std::exception& e) {
            std::cout << "Exception in vector test: " << e.what() << std::endl;
            results.push_back(false);
        }
    }

    int total_tests = static_cast<int>(results.size());
    int passed_tests = std::count(results.begin(), results.end(), true);
    
    std::cout << "\nFE Solver Test Summary: " << passed_tests << "/" << total_tests << " tests passed." << std::endl;
    
    return std::all_of(results.begin(), results.end(), [](bool v) { return v; });
}

} // namespace ScientificToolbox