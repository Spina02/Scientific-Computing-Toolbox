#include "../../include/ODE_Module/ODETester.hpp"
#include <cmath>
#include <functional>
#include <muParser.h>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iomanip>

namespace ScientificToolbox::ODE {

ODETester::ODETester() {
    load_tests_from_csv("../../data/ode_examples.csv");
}

bool ODETester::test_scalar_expression(const std::string& expr, double t_val, double y_val, double expected_val, int test_num) {
    try {
        auto scalar_f = parseScalarExpression(expr);
        double result = scalar_f(t_val, y_val);
        if (std::abs(result - expected_val) > 1e-4) {
            std::cout << "  Test " << test_num << " failed: Scalar function value is incorrect for expression: " << expr << std::endl;
            if (DEBUG)
                std::cout << "Expected: " << expected_val << ", Got: " << result << std::endl;
            return false;
        }
        std::cout << "  Test " << test_num << " passed" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "  Test " << test_num << " failed with exception: " << e.what() << std::endl;
        return false;
    }
}

bool ODETester::test_vector_expression(const vec_s& exprs, double t_val, const vec_d& y_val, const vec_d& expected_val, int test_num) {
    try {
        auto vector_f = parseVectorExpression(exprs);
        auto results = vector_f(t_val, y_val);
        for (Eigen::Index i = 0; i < results.size(); ++i) {
            if (std::abs(results[i] - expected_val[i]) > 1e-5) {
                std::cout << "  Test " << test_num << " failed: Vector function value is incorrect for expression: " << exprs[i] << std::endl;
                if (DEBUG)
                    std::cout << "Expected: " << expected_val.transpose() << ", Got: " << results.transpose() << std::endl;
                return false;
            }
        }
        std::cout << "  Test " << test_num << " passed" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "  Test " << test_num << " failed with exception: " << e.what() << std::endl;
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
            std::cout << "  Test " << test_num << " failed: For scalar expression, y_val and expected_val must be double." << std::endl;
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
            std::cout << "  Test " << test_num << " failed: For vector expression, y_val and expected_val must be VectorXd." << std::endl;
            return false;
        }

        const vec_d& y = std::get<vec_d>(y_val);
        const vec_d& exp_val = std::get<vec_d>(expected_val);

        return test_vector_expression(exprs, t_val, y, exp_val, test_num);

    } else {
        std::cout << "  Test " << test_num << " failed: Unknown expression type." << std::endl;
        return false;
    }
}

bool ODETester::test_simple_ode(const test_case& case_variant, const std::string solver_type, int test_num) {
    try {
        if (DEBUG) {
            std::cout << "\n=== Test " << test_num << " ===" << std::endl;
        }

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
        if (DEBUG){
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
            h /= 50; // Forward Euler Solver requires more steps
            sensitivity = 2e-3; // And more flexibility over the error
            solver = std::make_unique<ForwardEulerSolver>(f, t0, y0, tf, h);
        } else if (solver_type == "ExplicitMidpointSolver") {
            sensitivity = 1e-4;
            solver = std::make_unique<ExplicitMidpointSolver>(f, t0, y0, tf, h);
        }else if (solver_type == "RK4Solver") {
            sensitivity = 2e-8;
            solver = std::make_unique<RK4Solver>(f, t0, y0, tf, h);
        }  else {
            std::cout << "  Test " << test_num << " failed: Unknown solver type." << std::endl;
            return false;
        }

        auto results = solver->Solve();
        if (results.empty()) {
            std::cout << "  Test " << test_num << " failed: No results produced" << std::endl;
            return false;
        }

        // Print trajectory
        if (DEBUG) {
            std::cout << "\nSolution trajectory:" << std::endl;
            double t_cur = t0;
            int i = 0;
            
            // Save current format flags
            auto old_flags = std::cout.flags();
            auto old_precision = std::cout.precision();
            
            for (var_vec result : results) {
                if (i % static_cast<int>((tf-t0)/(h*10)) == 0) {
                    std::cout << std::fixed << std::setprecision(1);
                    std::cout << "t = " << t_cur << ", y = ";
                    // Set lower precision for trajectory
                    std::cout << std::fixed << std::setprecision(4);
                    if (auto* scalar_res = std::get_if<double>(&results[i])) {
                        std::cout << *scalar_res;
                    } else if (auto* vector_res = std::get_if<vec_d>(&results[i])) {
                        std::cout << vector_res->transpose();
                    }
                    std::cout << std::endl;
                }
                i++;
                t_cur += h;
            }

            // Restore original format
            std::cout.flags(old_flags);
            std::cout.precision(old_precision);

        }

        // Final comparison
        const var_vec& final_value = results.back();

        // Save current format flags
        auto old_flags = std::cout.flags();
        auto old_precision = std::cout.precision();        

        if (DEBUG){
            std::cout << "\nComparing final values:" << std::endl;
            // High precision for final comparison
            std::cout << std::scientific << std::setprecision(8);

            // Expected
            std::cout << "Expected: ";
            if (auto* scalar_exp = std::get_if<double>(&expected_final)) {
                std::cout << *scalar_exp << std::endl;
            } else if (auto* vector_exp = std::get_if<vec_d>(&expected_final)) {
                std::cout << vector_exp->transpose() << std::endl;
            }
        }

        // Got                
        if (DEBUG)
            std::cout << "Got:      ";
        
        if (auto* scalar_result = std::get_if<double>(&final_value)) {
            if (DEBUG){
                std::cout << *scalar_result << std::endl;
            }
            if (auto* scalar_exp = std::get_if<double>(&expected_final)) {
                double error = std::abs(*scalar_result - *scalar_exp);
                if (error > sensitivity) {
                    std::cout << "  Test " << test_num << " failed: error = " << error << std::endl;
                    return false;
                }
            } else {
                std::cout << "  Test " << test_num << " failed: type mismatch between expected and got." << std::endl;
                return false;
            }
        } else if (auto* vector_result = std::get_if<vec_d>(&final_value)) {
            if (auto* vector_exp = std::get_if<vec_d>(&expected_final)) {
                if (DEBUG){
                    std::cout << vector_result->transpose() << std::endl;
                }
                if (vector_result->size() != vector_exp->size()) {
                    std::cout << "  Test " << test_num << " failed: Vector size mismatch" << std::endl;
                    return false;
                }
                for (Eigen::Index i = 0; i < vector_result->size(); ++i) {
                    double error = std::abs((*vector_result)[i] - (*vector_exp)[i]);
                    if (error > sensitivity) {
                        std::cout << "  Test " << test_num << " failed: Vector value mismatch at index " << i << ". Error = " << error << std::endl;
                        return false;
                    }
                }
            } else {
                std::cout << "  Test " << test_num << " failed: type mismatch between expected and got." << std::endl;
                return false;
            }
        }

        // Restore original format
        std::cout.flags(old_flags);
        std::cout.precision(old_precision);

        std::cout << "  Test " << test_num << " passed" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cout << "  Test " << test_num << " failed with exception: " << e.what() << std::endl;
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
    
    return std::all_of(res.begin(), res.end(), [](bool v) { return v; });
}

bool ODETester::test_Solvers() {
    std::vector<bool> final_results;
    for (std::string solver_type : solver_types) {
        std::cout << "\nStarting " << solver_type << " Solver Tests\n" << std::endl;
        std::vector<bool> results;
        int test_counter = 1;

        // Scalar tests
        for (const auto& test : scalar_cases) {
            var_vec y0_var = test.y0;
            var_vec expected_var = test.expected_final;
            results.push_back(test_simple_ode(test, solver_type, test_counter++));
        }

        // Vector tests
        for (const auto& test : vector_cases) {
            try {
                var_vec y0_var = test.y0;
                var_vec expected_var = test.expected_final;
                results.push_back(test_simple_ode(test, solver_type, test_counter++));
            } catch (const std::exception& e) {
                std::cout << "Exception in vector test: " << e.what() << std::endl;
                results.push_back(false);
            }
        }

        int total_tests = static_cast<int>(results.size());
        int passed_tests = std::count(results.begin(), results.end(), true);
        
        std::cout << "\n" << solver_type << " Test Summary: " << passed_tests << "/" << total_tests << " tests passed." << std::endl;
        final_results.push_back(std::all_of(results.begin(), results.end(), [](bool v) { return v; }));
    }
    return std::all_of(final_results.begin(), final_results.end(), [](bool v) { return v; });
}

// In ODETester.cpp
void ODETester::load_tests_from_csv(const std::string& filename) {
    try {
        ImportCSV importer;
        
        std::cout << "Attempting to load tests from: " << filename << std::endl;

        // Clear existing test cases to prevent memory issues
        scalar_cases.clear();
        vector_cases.clear();
        
        // Check if file exists
        std::ifstream file(filename);
        if (!file.good()) {
            std::cout << "Warning: Test file " << filename << " not found. Using default tests." << std::endl;
            // Add default test cases
            scalar_cases.push_back({
                "y",        // dy/dt = y
                0.0,        // t0
                1.0,        // tf
                0.001,      // h
                1.0,        // y0
                std::exp(1.0), // Expected: e^t
                1.0         // Expected derivative at t0
            });

            // Add a simple vector test case
            vec_d y0(2), ef(2), ed(2);
            y0 << 1.0, 1.0;
            ef << std::exp(1.0), std::exp(1.0);
            ed << 1.0, 1.0;

            vector_cases.push_back({
                {"y0", "y1"},  // dy0/dt = y0, dy1/dt = y1
                0.0,           // t0
                1.0,           // tf
                0.001,         // h
                y0,            // y0
                ef,            // expected_final
                ed            // expected_derivative
            });
            return;
        }
        file.close();

        try {
            importer.import(filename);
            auto data = importer.getData();
            
            if (data.empty()) {
                std::cout << "Warning: No test cases found in " << filename << std::endl;
                return;
            }

            std::cout << "Found " << data.size() << " test cases in file" << std::endl;

            for (const auto& row : data) {
                try {
                    parse_test_case(row);
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing row: " << e.what() << std::endl;
                    continue;
                }
            }
            
            std::cout << "Successfully loaded " << scalar_cases.size() << " scalar and " 
                      << vector_cases.size() << " vector test cases." << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Error importing CSV: " << e.what() << std::endl;
            throw;
        }

    } catch (const std::exception& e) {
        std::cerr << "Fatal error loading test cases: " << e.what() << std::endl;
        throw;
    }
}

void ODETester::parse_test_case(const std::unordered_map<std::string, OptionalDataValue>& row) {
    try {
        if (!row.count("type") || !row.at("type").has_value()) {
            std::cerr << "Warning: Skipping row without type" << std::endl;
            return;
        }

        std::string type = std::get<std::string>(row.at("type").value());

        const std::vector<std::string> required_fields = {
            "expr", "t0", "tf", "h", "y0", "expected_final", "expected_derivative"
        };

        // Check required fields
        for (const auto& field : required_fields) {
            if (!row.count(field) || !row.at(field).has_value()) {
                std::cerr << "Warning: Skipping row missing required field: " << field << std::endl;
                return;
            }
        }

        if (type == "scalar") {
            ScalarODETestCase test;
            test.expr = std::get<std::string>(row.at("expr").value());
            test.t0 = std::get<double>(row.at("t0").value());
            test.tf = std::get<double>(row.at("tf").value());
            test.h = std::get<double>(row.at("h").value());
            test.y0 = std::get<double>(row.at("y0").value());
            test.expected_final = std::get<double>(row.at("expected_final").value());
            test.expected_derivative = std::get<double>(row.at("expected_derivative").value());
            scalar_cases.push_back(test);
        } 
        else if (type == "vector") {
            VectorODETestCase test;
            
            // Parse comma-separated expressions
            std::string expr_str = std::get<std::string>(row.at("expr").value());
            std::stringstream ss(expr_str);
            std::string expr;
            while (std::getline(ss, expr, ',')) {
                test.exprs.push_back(expr);
            }

            if (test.exprs.empty()) {
                std::cerr << "Warning: Skipping vector test case with no expressions" << std::endl;
                return;
            }

            test.t0 = std::get<double>(row.at("t0").value());
            test.tf = std::get<double>(row.at("tf").value());
            test.h = std::get<double>(row.at("h").value());

            // Parse y0 vector
            vec_d y0(test.exprs.size());
            //! here the error
            std::string y0_str = std::get<std::string>(row.at("y0").value());
            std::stringstream ss_y0(y0_str);
            std::string val;
            size_t i = 0;
            while (std::getline(ss_y0, val, ',') && i < test.exprs.size()) {
                y0(i++) = std::stod(val);
            }
            test.y0 = y0;

            if (i != test.exprs.size()) {
                std::cerr << "Warning: Mismatch between number of expressions and y0 values" << std::endl;
                return;
            }

            // Parse expected_final vector
            vec_d expected_final(test.exprs.size());
            std::string expected_final_str = std::get<std::string>(row.at("expected_final").value());
            std::stringstream ss_expected_final(expected_final_str);
            i = 0;
            while (std::getline(ss_expected_final, val, ',') && i < test.exprs.size()) {
                expected_final(i++) = std::stod(val);
            }
            test.expected_final = expected_final;

            if (i != test.exprs.size()) {
                std::cerr << "Warning: Mismatch between number of expressions and expected_final values" << std::endl;
                return;
            }

            // Parse expected_derivative vector
            vec_d expected_derivative(test.exprs.size());
            std::string expected_derivative_str = std::get<std::string>(row.at("expected_derivative").value());
            std::stringstream ss_expected_derivative(expected_derivative_str);
            i = 0;
            while (std::getline(ss_expected_derivative, val, ',') && i < test.exprs.size()) {
                expected_derivative(i++) = std::stod(val);
            }
            test.expected_derivative = expected_derivative;

            if (i != test.exprs.size()) {
                std::cerr << "Warning: Mismatch between number of expressions and expected_derivative values" << std::endl;
                return;
            }

            vector_cases.push_back(test);
        }
        else {
            std::cerr << "Warning: Unknown test type: " << type << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing test case: " << e.what() << std::endl;
    }
}

} // namespace ScientificToolbox