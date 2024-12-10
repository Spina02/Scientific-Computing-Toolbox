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

bool ODETester::test_expression(const var_expr& expr_variant, double t_val, const var_vec& y_val, const var_vec& expected_val, int test_num) {
    Func f{parseExpression(expr_variant)};
    var_vec result = f(t_val, y_val);
    if (result != expected_val) {
        std::cout << "  Test " << test_num << " failed: Value is incorrect" <<std::endl;
        if (DEBUG) {
            std::visit([](const var_vec& res, const var_vec& ex) {
                std::cout << "    Expected: " << ex << ", Got: " << res << std::endl;
            }, result, expected_val);
        }
    }
    std::cout << "  Test " << test_num << " passed" << std::endl;
    return true;
}

bool ODETester::test_expression_parser() {
    std::vector<bool> res;
    int test_counter = 1;

    std::cout << "\nStarting Expression Parser Tests\n" << std::endl;

    for (const auto& test : cases) {
        res.push_back(test_expression(test.expr,test.t0,test.y0,test.expected_derivative,test_counter++));
    }

    // Print summary
    int total_tests = static_cast<int>(res.size());
    int passed_tests = std::count(res.begin(), res.end(), true);
    
    std::cout << "\nTest Summary: " << passed_tests << "/" << total_tests 
              << " tests passed." << std::endl;
    
    return std::all_of(res.begin(), res.end(), [](bool v) { return v; });
}

bool ODETester::test_simple_ode(const ODETestCase& test_case, const std::string solver_type, int test_num) {
    if (DEBUG) {
        std::cout << "\n=== Test " << test_num << " ===" << std::endl;
    }

    var_expr expr_variant;
    double t0, tf, h;
    var_vec y0, expected_final;

    
    expr_variant = test_case.expr;
    t0 = test_case.t0;
    tf = test_case.tf;
    h = test_case.h;
    y0 = test_case.y0;
    expected_final = test_case.expected_final;


    if (DEBUG) {
        std::cout << "ODE: " << expr_variant << std::endl;

        std::cout << "t0 = " << t0 << ", tf = " << tf << ", h = " << h << std::endl;

        // Print initial conditions
        std::cout << "Initial condition: " << y0 << std::endl;
    }


    Func f {parseExpression(expr_variant)};
    std::unique_ptr<ODESolver> solver = nullptr;
    double sensitivity = 0.0;

    if (solver_type == "ForwardEulerSolver") {
        h /= 50;
        sensitivity = 2e-3;
        solver = std::make_unique<ForwardEulerSolver>(f, t0, y0, tf, h);
    } else if (solver_type == "ExplicitMidpointSolver") {
        sensitivity = 1e-4;
        solver = std::make_unique<ExplicitMidpointSolver>(f, t0, y0, tf, h);
    } else if (solver_type == "RK4Solver") {
        sensitivity = 2e-8;
        solver = std::make_unique<RK4Solver>(f, t0, y0, tf, h);
    } else {
        std::cout << "  Test " << test_num << " failed: Unknown solver type." << std::endl;
        return false;
    }

    auto results = solver->Solve();
    if (results.empty()) {
        std::cout << "  Test " << test_num << " failed: No results produced" << std::endl;
        return false;
    }

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

    const var_vec& final_value = results.back();

    // Save current format flags
    auto old_flags = std::cout.flags();
    auto old_precision = std::cout.precision();

    if (DEBUG) {
        std::cout << "\nComparing final values:" << std::endl;
        // High precision for final comparison
        std::cout << std::scientific << std::setprecision(8);

        // Expected
        std::cout << "Expected: " << expected_final << std::endl;
        std::cout << "Got:      " << final_value << std::endl;
    }

    // Restore original format
    std::cout.flags(old_flags);
    std::cout.precision(old_precision);

    // Check final value
    if (auto* scalar_result = std::get_if<double>(&final_value)) {
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
    } else {
        std::cout << "  Test " << test_num << " failed: unknown type for final value." << std::endl;
        return false;
    }

    std::cout << "  Test " << test_num << " passed" << std::endl;
    return true;
}

/*
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
*/

bool ODETester::run_tests() {
    std::vector<bool> final_results;
    for (std::string solver_type : solver_types) {
        std::cout << "\nStarting " << solver_type << " Solver Tests\n" << std::endl;
        std::vector<bool> results;
        int test_counter = 1;

        // Scalar tests
        for (const auto& test : cases) {
            var_vec y0_var = test.y0;
            var_vec expected_var = test.expected_final;
            results.push_back(test_simple_ode(test, solver_type, test_counter++));
        }

        int total_tests = static_cast<int>(results.size());
        int passed_tests = std::count(results.begin(), results.end(), true);
        
        std::cout << "\n" << solver_type << " Test Summary: " << passed_tests << "/" << total_tests << " tests passed." << std::endl;
        final_results.push_back(std::all_of(results.begin(), results.end(), [](bool v) { return v; }));
    }
    return std::all_of(final_results.begin(), final_results.end(), [](bool v) { return v; });
}

} // namespace ScientificToolbox