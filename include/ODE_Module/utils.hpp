#ifndef UTILITIES_ODE_HPP
#define UTILITIES_ODE_HPP

#include "../../include/Utilities/ImportData.hpp"
#include "../../include/Utilities/ImportCSV.hpp"
#include "types.hpp"

namespace ScientificToolbox::ODE {

inline const bool DEBUG = true;

// Parse equation(s) from string(s)
var_func parseExpression(const var_expr& expr);

// Function to read and write CSV file
void save_on_CSV(const std::string& filename, const ODESolution& solution);
void load_tests_from_csv(const std::string& filename);
void parse_test_case(const std::unordered_map<std::string, OptionalDataValue>& row);

// Test cases
extern std::vector<ODETestCase> cases;

} // namespace ScientificToolbox::ODE

#endif // UTILITIES_ODE_HPP