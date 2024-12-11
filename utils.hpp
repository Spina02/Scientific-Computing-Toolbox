
#ifndef UTILITIES_ODE_HPP
#define UTILITIES_ODE_HPP

#include "../../include/Utilities/ImportData.hpp"
#include "../../include/Utilities/ImportCSV.hpp"
#include "types.hpp"

/**
 * @namespace ScientificToolbox::ODE
 * @brief Namespace containing utilities for Ordinary Differential Equations (ODE) handling
 */
namespace ScientificToolbox::ODE {

inline const bool DEBUG = false;

/**
 * @brief Parses a mathematical expression into a callable function
 * @param expr The expression to parse
 * @return Parsed function that can be evaluated
 */
var_func parseExpression(const var_expr& expr);

/**
 * @brief Saves ODE solution data to a CSV file
 * @param filename The name of the file to save to
 * @param solution The ODE solution data to save
 */
void save_on_CSV(const std::string& filename, const ODESolution& solution);

/**
 * @brief Loads test cases from a CSV file
 * @param filename The name of the file to load from
 */
void load_tests_from_csv(const std::string& filename);

/**
 * @brief Parses a single test case from CSV row data
 * @param row Map containing the test case data
 */
void parse_test_case(const std::unordered_map<std::string, OptionalDataValue>& row);

/**
 * @brief Vector containing all ODE test cases
 */
extern std::vector<ODETestCase> cases;

} // namespace ScientificToolbox::ODE

#endif // UTILITIES_ODE_HPP