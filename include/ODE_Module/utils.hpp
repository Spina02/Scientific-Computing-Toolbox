#ifndef UTILITIES_ODE_HPP
#define UTILITIES_ODE_HPP

#include "../../include/Utilities/ImportData.hpp"
#include "../../include/Utilities/ImportCSV.hpp"
#include "types.hpp"

namespace ScientificToolbox::ODE {

inline const bool DEBUG = false;

// Function to read and write CSV file
void save_on_CSV(const std::string& filename, const std::vector<std::vector<double>>& data, const std::vector<std::string>& headers);
void load_tests_from_csv(const std::string& filename);
void parse_test_case(const std::unordered_map<std::string, OptionalDataValue>& row);

// Overload for printing variant types
std::ostream& operator<<(std::ostream& os, const var_vec& vec);
std::ostream& operator<<(std::ostream& os, const var_expr& expr);

// Overload for arithmetic operations
var_vec operator*(double h, const var_vec& v);
var_vec operator+(const var_vec& v1, const var_vec& v2);
var_vec operator-(const var_vec& v1, const var_vec& v2);
var_vec operator/(const var_vec& v1, const double v2);
var_vec operator/(const var_vec& v1, const var_vec& v2);

// Test cases
extern std::vector<ScalarODETestCase> scalar_cases;
extern std::vector<VectorODETestCase> vector_cases;
extern std::vector<ODETestCase> cases;

} // namespace ScientificToolbox::ODE

#endif // UTILITIES_ODE_HPP