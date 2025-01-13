#include "../../include/ODE_Module/ODETester.hpp"
#include "../../include/ODE_Module/analysis.hpp"
#include <cmath>
#include <functional>
#include <muParser.h>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <filesystem>

namespace ScientificToolbox::ODE {

ODETester::ODETester() {
    std::string project_dir = std::filesystem::current_path();
    load_tests_from_csv(project_dir + "/data/ode_tests.csv");
}

bool ODETester::test_expression(ODETestCase test, int test_num) const {
    if (DEBUG) {
        std::cout << std::endl << " ----------------- Test " << test_num << " -----------------" << std::endl << std::endl;
    }

    // Extract test case data
    var_expr expr_variant = test.expr;
    double t_val = test.t0;
    var_vec y_val = test.y0;
    var_vec expected_val;
    if (!test.expected_derivative.has_value()) {
        std::cout << "  Test " << test_num << " failed: Expected derivative not provided" << std::endl;
        return false;
    } else {
        expected_val = test.expected_derivative.value();
    }

    Func f{parseExpression(expr_variant), expr_variant};
    var_vec result = f(t_val, y_val);
    if (DEBUG) {
        std::cout << "  Expression: " << expr_variant << std::endl;
        std::cout << "  t =         " << t_val << std::endl;
        std::cout << "  y =         " << y_val << std::endl << std::endl;
        std::visit([](const var_vec& res, const var_vec& ex) {
            std::cout << "  Expected:   " << ex << "\n  Got:        " << res << std::endl << std::endl;
        }, result, expected_val);
    }
    if (!almost_equal(result, expected_val)) {
        std::cout << "  Result: " << result << std::endl << "  Expected: " << expected_val << std::endl;
        std::cout << "  Test " << test_num << " failed: Value is incorrect" <<std::endl;
        if (DEBUG)
            std::cout << std::endl << "------------------------------------------" << std::endl;
        return false;
    }
    std::cout << "  Test " << test_num << " passed" << std::endl;

    if (DEBUG)
        std::cout << std::endl << "------------------------------------------" << std::endl;

    return true;
}

bool ODETester::run_parser_tests() const {
    std::vector<bool> res;
    int test_counter = 1;

    std::cout << std::endl << std::endl;
    if (DEBUG) {
            std::cout << std::endl;
            std::cout << "╔══════════════════════════════════════════════════════╗" << std::endl;
            std::cout << "║           ";
        }
        std::cout << "Starting Expression Parser Tests";
        if (DEBUG) {
            std::cout << "           ║" << std::endl;
            std::cout << "╚══════════════════════════════════════════════════════╝" << std::endl << std::endl;
        } else {
            std::cout << std::endl << std::endl;
        }

    for (const auto& test : cases) {
        res.push_back(test_expression(test, test_counter++));
    }

    // Print summary
    int total_tests = static_cast<int>(res.size());
    int passed_tests = std::count(res.begin(), res.end(), true);
    
    std::cout << "\nTest Summary: " << passed_tests << "/" << total_tests << " tests passed." << std::endl << std::endl;
    
    return std::all_of(res.begin(), res.end(), [](bool v) { return v; });
}

bool ODETester::test_simple_ode(const ODETestCase& test_case, const std::string solver_type, int test_num) const {
    if (DEBUG) {
        std::cout << std::endl << " ----------------- Test " << test_num << " -----------------" << std::endl << std::endl;
    }

    var_expr expr_variant;
    double t0, tf, h;
    var_vec y0, expected_solution;

    
    expr_variant = test_case.expr;
    t0 = test_case.t0;
    tf = test_case.tf;
    h = test_case.h;
    y0 = test_case.y0;
    if (!test_case.expected_solution.has_value()) {
        std::cout << "  Test " << test_num << " failed: Expected value not provided" << std::endl;
        return false;
    } else {
        expected_solution = test_case.expected_solution.value();
    }


    if (DEBUG) {
        std::cout << "  ODE: " << expr_variant << std::endl;

        std::cout << "  t0 = " << t0 << ", tf = " << tf << ", h = " << h << std::endl;

        // Print initial conditions
        std::cout << "  y0 = " << y0 << std::endl;
    }

    std::unique_ptr<ODESolver> solver = nullptr;
    double sensitivity = 0;
    if (solver_type == "ForwardEulerSolver") {
        h /= 100; // Lower h for Forward Euler
        sensitivity = 2e-3;
        solver = std::make_unique<ForwardEulerSolver>(expr_variant, y0, t0, tf, h);
    } else if (solver_type == "ExplicitMidpointSolver") {
        sensitivity = 1e-4;
        solver = std::make_unique<ExplicitMidpointSolver>(expr_variant, y0, t0, tf, h);
    } else if (solver_type == "RK4Solver") {
        sensitivity = 1e-8;
        solver = std::make_unique<RK4Solver>(expr_variant, y0, t0, tf, h);
    } else {
        std::cout << "  Test " << test_num << " failed: Unknown solver type." << std::endl;
        return false;
    }

    auto solution = solver->solve();
    const auto& results = solution.y_values;
    if (results.empty()) {
        std::cout << "  Test " << test_num << " failed: No results produced" << std::endl;
        return false;
    }

    if (DEBUG)
        std::cout << solution << std::endl;

    const var_vec& final_value = results.back();

    double error = compute_error(final_value, expected_solution);

    if (DEBUG) {
        // Save current format flags
        auto old_flags = std::cout.flags();
        auto old_precision = std::cout.precision();
        // High precision for final comparison
        std::cout << std::scientific << std::setprecision(8);

        std::cout << "  Comparing final values:" << std::endl;
        std::cout << std::endl;
        std::cout << "    Expected: " << expected_solution << std::endl;
        std::cout << "    Got:      " << final_value << std::endl;
        std::cout << "    Error: " << error << std::endl;
        std::cout << std::endl;

        // Restore original format
        std::cout.flags(old_flags);
        std::cout.precision(old_precision);
    }

    // Check final value
    if (DEBUG) {
        
    }
    if (error > sensitivity) {
        std::cout << "  Test " << test_num << " failed: error = " << error << std::endl;
        return false;
    }

    std::cout << "  Test " << test_num << " passed" << std::endl;
    return true;
}

bool ODETester::run_ode_tests() const {
    std::vector<bool> final_results;
    for (std::string solver_type : solver_types) {
        if (DEBUG) {
            std::cout << "╔══════════════════════════════════════════════════════╗" << std::endl;
            std::cout << "║" << std::string((32 - solver_type.length())/2, ' ');
        }
        std::cout << "Starting " << solver_type << " Solver Tests";
        if (DEBUG) {
            std::cout << std::string((32 - solver_type.length() + 1)/2, ' ') << "║" << std::endl;
            std::cout << "╚══════════════════════════════════════════════════════╝" << std::endl;
        } else {
            std::cout << std::endl << std::endl;
        }
        std::vector<bool> results;
        int test_counter = 1;

        // Scalar tests
        for (const auto& test : cases) {
            results.push_back(test_simple_ode(test, solver_type, test_counter++));
        }

        int total_tests = static_cast<int>(results.size());
        int passed_tests = std::count(results.begin(), results.end(), true);
        
        if (DEBUG)
            std::cout << std::endl << "------------------------------------------" << std::endl;
        std::cout << std::endl << solver_type << " Test Summary: " << passed_tests << "/" << total_tests << " tests passed." << std::endl << std::endl;
        final_results.push_back(std::all_of(results.begin(), results.end(), [](bool v) { return v; }));
    }
    return std::all_of(final_results.begin(), final_results.end(), [](bool v) { return v; });
}

} // namespace ScientificToolbox