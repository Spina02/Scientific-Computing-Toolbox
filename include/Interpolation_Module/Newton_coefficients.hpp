#ifndef NEWTON_COEFFICIENTS_HPP
#define NEWTON_COEFFICIENTS_HPP

namespace ScientificToolbox{
    #include <vector>
    #include <cmath> // For std::isnan

    template <typename T>
    std::vector<std::vector<T>> divided_differences(const std::vector<T>& x, const std::vector<T>& y) {
        size_t n = x.size();
        std::vector<std::vector<T>> table(n, std::vector<T>(n, 0.0)); // Table of divided differences

        // Initialize the first column with y values
        for (size_t i = 0; i < n; ++i) {
            table[i][0] = y[i];
        }

        // Compute divided differences
        for (size_t j = 1; j < n; ++j){
            for (size_t i = 0; i < n - j; ++i) {
                table[i][j] = (table[i + 1][j - 1] - table[i][j - 1]) / (x[i + j] - x[i]);
            }
        }

        return table;
    }

    template <typename T>
    // Function to compute Newton polynomial coefficients
    std::vector<T> newton_coefficients(const std::vector<T>& x, const std::vector<T>& y) {
        auto table = divided_differences(x, y);
        std::vector<T> coefficients;

        // Extract coefficients from the divided differences table
        for (size_t i = 0; i < table.size(); ++i) {
            coefficients.push_back(table[0][i]);
        }

        return coefficients;
    }
}

#endif