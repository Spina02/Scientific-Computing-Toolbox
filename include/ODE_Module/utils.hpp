#ifndef UTILITIES_ODE_HPP
#define UTILITIES_ODE_HPP

#include "../../include/Utilities/ImportData.hpp"
#include "../../include/Utilities/ImportCSV.hpp"
#include "types.hpp"

namespace ScientificToolbox::ODE {

inline const bool DEBUG = true;

// Function to read and write CSV file
void save_on_CSV(const std::string& filename, const std::vector<std::vector<double>>& data, const std::vector<std::string>& headers);
void load_tests_from_csv(const std::string& filename);
void parse_test_case(const std::unordered_map<std::string, OptionalDataValue>& row);

// Overload for printing Eigen::VectorXd
std::ostream& operator<<(std::ostream& os, const var_vec& vec);

// Test cases
extern std::vector<ScalarODETestCase> scalar_cases;
extern std::vector<VectorODETestCase> vector_cases;
extern test_case cases;

} // namespace ScientificToolbox::ODE

#endif // UTILITIES_ODE_HPP