#include "../../include/ODE_Module/utils.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <muParser.h>
#include <memory>

namespace ScientificToolbox::ODE {

std::vector<ODETestCase> cases;

var_func parseExpression(const var_expr& ex) {
    try {
        try {
            std::string expr = std::get<std::string>(ex);
            if (expr.empty()) {
                throw std::runtime_error("The expression is empty.");
            }
            // shared pointer for `t` and `y`
            auto parser = std::make_shared<mu::Parser>();
            auto t_ptr = std::make_shared<double>(0.0);
            auto y_ptr = std::make_shared<double>(0.0);

            // initialize parser
            parser->DefineVar("t", t_ptr.get());
            parser->DefineVar("y", y_ptr.get());
            parser->SetExpr(expr);

            // Lambda function that evaluates the expression
            return [parser, t_ptr, y_ptr](double t, double y) -> double {
                *t_ptr = t;
                *y_ptr = y;
                return parser->Eval();
            };
        } catch (...) {
            vec_s exprs = std::get<vec_s>(ex);
                auto t_ptr = std::make_shared<double>(0.0);
            auto y_ptrs = std::make_shared<std::vector<std::shared_ptr<double>>>();
            y_ptrs->resize(exprs.size());

            std::vector<std::shared_ptr<mu::Parser>> parsers(exprs.size());

            for (size_t i = 0; i < exprs.size(); ++i) {
                if (exprs[i].empty()) {
                    throw std::runtime_error("Expression " + std::to_string(i) + " is empty.");
                }

                parsers[i] = std::make_shared<mu::Parser>();
                parsers[i]->DefineVar("t", t_ptr.get());

                // For single component systems, allow using just 'y'
                if (exprs.size() == 1) {
                    if (!y_ptrs->at(0)) {
                        y_ptrs->at(0) = std::make_shared<double>(0.0);
                    }
                    parsers[i]->DefineVar("y", y_ptrs->at(0).get());
                }

                // Always define y0,y1,... style variables
                for (size_t j = 0; j < exprs.size(); ++j) {
                    if (!y_ptrs->at(j)) {
                        y_ptrs->at(j) = std::make_shared<double>(0.0);
                    }
                    parsers[i]->DefineVar("y" + std::to_string(j+1), y_ptrs->at(j).get());
                }
                parsers[i]->SetExpr(exprs[i]);
            }

            // Lambda function that evaluates the expressions
            return [parsers, t_ptr, y_ptrs](double t, const vec_d &y) -> vec_d {
                if (static_cast<size_t>(y.size()) != y_ptrs->size()) {
                    throw std::runtime_error("Mismatch between number of expressions and size of y vector.");
                }

                // Update the value of `t`
                *t_ptr = t;

                // Update the value of each `y`
                for (Eigen::Index i = 0; i < y.size(); ++i) {
                    *(y_ptrs->at(i)) = y[i];
                }

                // Evaluate each expression
                vec_d result(parsers.size());
                for (size_t i = 0; i < parsers.size(); ++i) {
                    result(i) = parsers[i]->Eval();
                }

                return result;
            };
        }
    } catch (const mu::Parser::exception_type& e) {
        throw std::runtime_error("Error parsing vector expression: " + std::string(e.GetMsg()));
    }
}

void save_on_CSV(const std::string& filename, const ODESolution& solution) {
    int n = solution.size;
    // create folder it does not exist
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    // open file
    std::ofstream file(filename);
    // check if file is open
    if (!file.is_open())
        throw std::runtime_error("Could not open file for writing.");

    var_vecs results = solution.y_values;

    vec_s headers = {"t"};
    // add headers for additional components
    if (std::holds_alternative<vec_d>(results[0]))
        for (Eigen::Index i = 0; i < std::get<vec_d>(results[0]).size(); ++i)
            headers.push_back("y" + std::to_string(i+1));
    else
        headers.push_back("y");


    // write headers
    for (size_t i = 0; i < headers.size(); ++i) {
        file << headers[i];
        if (i < headers.size() - 1) {
            file << ",";
        }
    }
    file << std::endl;
    // write data
    for (Eigen::Index i = 0; i <= n; ++i) {
        file << solution.t_values[i] << ",";
        if (std::holds_alternative<double>(results[i]))
            file << std::get<double>(results[i]);
        else {
            const auto& vec = std::get<vec_d>(results[i]);
            for (Eigen::Index j = 0; j < vec.size(); ++j) {
                file << vec[j];
                if (j < vec.size() - 1)
                    file << ",";
            }
        }
        file << std::endl;
    }

    // close file
    file.close();
}

std::vector<ODETestCase> load_tests_from_csv(const std::string& filename) {
    try {
        Importer importer;
        
        std::cout << "Attempting to load tests from: " << filename << std::endl;

        // Clear existing test cases
        cases.clear();
        
        // Check if file exists
        std::ifstream file(filename);
        if (!file.good()) {
            std::cerr << "Error: File not found: " << filename << std::endl;
            return std::vector<ODETestCase>{};
        }
        file.close();

        try {
            importer.import(filename);
            auto data = importer.getData();
            
            if (data.empty()) {
                std::cout << "Warning: No test cases found in " << filename << std::endl;
                return std::vector<ODETestCase>{};
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
            
            std::cout << "\nSuccessfully loaded " << cases.size() << " test cases" << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Error importing CSV: " << e.what() << std::endl;
            throw;
        }

    } catch (const std::exception& e) {
        std::cerr << "Fatal error loading test cases: " << e.what() << std::endl;
        throw;
    }

    return cases;
}

var_expr parse_var_expr(const std::string& str) {
    std::stringstream ss(str);
    vec_s exprs;
    std::string expr;
    while (std::getline(ss, expr, ',')) {
        exprs.push_back(expr);
    }
    if (exprs.size() == 1) {
        return exprs[0];
    }
    return exprs;
}

var_vec parse_var_vec(const std::variant<std::string, double> value) {
    // rty to return a double
    if (std::holds_alternative<double>(value)) {
        return std::get<double>(value);
    } else {
        std::string str = std::get<std::string>(value);
        std::stringstream ss(str);
        vec_d vec;
        std::string val;
        while (std::getline(ss, val, ',')) {
            vec.conservativeResize(vec.size() + 1);
            vec(vec.size() - 1) = std::stod(val);
        }
        return vec;
    }
}

void parse_test_case(const std::unordered_map<std::string, OptionalDataValue>& row) {
    try {
        if (!row.count("type") || !row.at("type").has_value()) {
            std::cerr << "Warning: Skipping row without type" << std::endl;
            return;
        }

        std::string type = std::get<std::string>(row.at("type").value());

        const std::vector<std::string> required_fields = {
            "expr", "t0", "tf", "h", "y0"
        };

        // Check required fields
        for (const auto& field : required_fields) {
            if (!row.count(field) || !row.at(field).has_value()) {
                std::cerr << "Warning: Skipping row missing required field: " << field << std::endl;
                return;
            }
        }

        ODETestCase test;
        test.expr = parse_var_expr(std::get<std::string>(row.at("expr").value()));
        test.t0 = std::get<double>(row.at("t0").value());
        test.tf = std::get<double>(row.at("tf").value());
        test.h = std::get<double>(row.at("h").value());
        try {
            test.y0 = parse_var_vec(std::get<double>(row.at("y0").value()));
            try {
                test.expected_final = parse_var_vec(std::get<double>(row.at("expected_final").value()));
                test.expected_derivative = parse_var_vec(std::get<double>(row.at("expected_derivative").value()));
            } catch (...) {
                test.expected_final = std::nullopt;
                test.expected_derivative  = std::nullopt;
            }
        } catch (...) {
            test.y0 = parse_var_vec(std::get<std::string>(row.at("y0").value()));
            try {
                test.expected_final = parse_var_vec(std::get<std::string>(row.at("expected_final").value()));
                test.expected_derivative = parse_var_vec(std::get<std::string>(row.at("expected_derivative").value()));
            } catch (...) {
                test.expected_final = std::nullopt;
                test.expected_derivative  = std::nullopt;
            }
        }

        cases.push_back(test);

    } catch (const std::exception& e) {
        std::cerr << "Error parsing test case: " << e.what() << std::endl;
    }
}

}