#include "../../include/ODE_Module/utils.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace ScientificToolbox::ODE {
    std::vector<ScalarODETestCase> scalar_cases;
    std::vector<VectorODETestCase> vector_cases;

    // Overload for printing var_vec custom type
    std::ostream& operator<<(std::ostream& os, const var_vec& vec) {
        if (std::holds_alternative<double>(vec)) {
            os << std::get<double>(vec);
        } else {
            const Eigen::VectorXd& v = std::get<Eigen::VectorXd>(vec);
            os << "[";
            for (int i = 0; i < v.size(); ++i) {
                os << v(i);
                if (i < v.size() - 1) {
                    os << ", ";
                }
            }
            os << "]";
        }
        return os;
    }

    void save_on_CSV(const std::string& filename, const std::vector<std::vector<double>>& data, const std::vector<std::string>& headers) {

        // create folder it does not exist
        std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
        // open file
        std::ofstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for writing.");
        }

        // write headers
        for (size_t i = 0; i < headers.size(); ++i) {
            file << headers[i];
            if (i < headers.size() - 1) {
                file << ",";
            }
        }
        file << std::endl;

        // write data
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data[i].size(); ++j) {
                file << data[i][j];
                if (j < data[i].size() - 1) {
                    file << ",";
                }
            }
            file << std::endl;
        }

        // close file
        file.close();
    }

void load_tests_from_csv(const std::string& filename) {
    try {
        ImportCSV importer;
        
        std::cout << "Attempting to load tests from: " << filename << std::endl;

        // Clear existing test cases
        scalar_cases.clear();
        vector_cases.clear();
        
        // Check if file exists
        std::ifstream file(filename);
        if (!file.good()) {
            std::cout << "Warning: Test file " << filename << " not found. Using default tests." << std::endl;
            // Add default test cases
            scalar_cases.push_back({
                "y",           // dy/dt = y
                0.0,           // t0
                1.0,           // tf
                0.001,         // h
                1.0,           // y0
                std::exp(1.0), // Expected: e^t
                1.0            // Expected derivative at t0
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
                ed             // expected_derivative
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

void parse_test_case(const std::unordered_map<std::string, OptionalDataValue>& row) {
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
}