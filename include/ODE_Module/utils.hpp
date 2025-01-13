#ifndef UTILITIES_ODE_HPP
#define UTILITIES_ODE_HPP

/**
 * @file utils.hpp
 * @brief Utility functions for ODE module operations
 * 
 * This module provides common utilities for the ODE solver framework including:
 * - Expression parsing functionality
 * - File I/O operations for CSV data
 * - Test case management
 * - Debug configuration
 */

#include "../../include/Utilities.hpp"
#include "types.hpp"

namespace ScientificToolbox::ODE {

inline const bool DEBUG = false;

/** ### parseExpression
 * @brief Parse a mathematical expression into a callable function
 * @param expr Mathematical expression to parse of the form `f(t, y)` or `[f1(t, y0, y1, ...), f2(t, y0, y1, ...), ...]`
 * @return `var_func` Callable function representing the expression
 */
var_func parseExpression(const var_expr& expr);

/** ### parse_var_expr
 * @brief Parse a string into a var_expr type (std::variant<std::vector<std::string>, std::string>)
 * 
 * @param str The string representing the expression(s)
 * @return `var_expr` The result
 */
var_expr parse_var_expr(const std::string& str);

/** ### save_to_csv
 * @brief Save ODE solution data to a CSV file
 * @param filename Path to the output CSV file
 * @param solution ODE solution data to save
 */
void save_to_csv(const std::string& filename, const ODESolution& solution, bool append = false);

/** ### load_tests_from_csv
 * @brief Load test cases from a CSV file
 * @param filename Path to the CSV file containing test cases
 */
std::vector<ODETestCase> load_tests_from_csv(const std::string& filename);

/** ### parse_test_case
 * @brief Parse a test case from a row of data
 * @param row Unordered map representing a row of data
 */
void parse_test_case(const std::unordered_map<std::string, OptionalDataValue>& row);

// Test cases
extern std::vector<ODETestCase> cases;

} // namespace ScientificToolbox::ODE

#endif // UTILITIES_ODE_HPP