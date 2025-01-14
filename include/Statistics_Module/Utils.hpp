#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <unordered_map>
#include <optional>
#include <variant>
#include <stdexcept>
#include <iostream>
#include <type_traits>

using DataValue = std::variant<int, double, std::string>;
using OptionalDataValue = std::optional<DataValue>;

namespace ScientificToolbox::Utils {

/**
 * @brief Extracts a column of specified type from a dataset structured as a vector of maps.
 * 
 * @details
 *   This template function processes a dataset where each row is represented as an 
 *   unordered_map with string keys and OptionalDataValue values. It extracts values of type T 
 *   from a specified column, including proper handling of missing values, numeric conversions
 *   (int <-> double), and string extraction.
 * 
 * @tparam T The desired output type for the extracted column data. 
 *           Supported types in the variant are: int, double, and std::string.
 * 
 * @param data A vector of unordered maps representing the dataset, where each map is a row.
 * @param columnName The string identifier for the column to extract.
 * 
 * @return std::vector<T> A vector containing the extracted column values of type T.
 * 
 * @throws std::runtime_error if:
 *   - The input data vector is empty.
 *   - The specified column name does not exist in the dataset.
 *   - No valid data of the requested type is found in the column (i.e., empty result).
 * 
 * @note The function will skip invalid or incompatible values without throwing immediately, 
 *       but will throw if *no* valid values are found at all.
 */
template <typename T>
std::vector<T> extractColumn(
    const std::vector<std::unordered_map<std::string, OptionalDataValue>>& data,
    const std::string& columnName)
{
    if (data.empty()) {
        throw std::runtime_error("Data is empty");
    }

    
    if (data.front().find(columnName) == data.front().end()) {
        throw std::runtime_error("Column '" + columnName + "' does not exist");
    }

    std::vector<T> columnData;
    columnData.reserve(data.size()); 
    for (const auto& row : data) {
        auto it = row.find(columnName);
        if (it != row.end() && it->second.has_value()) {
            try {
                const auto& value = it->second.value();

             
                if (std::holds_alternative<T>(value)) {
                    columnData.push_back(std::get<T>(value));
                }
            
                else if constexpr (std::is_arithmetic_v<T>) {
                    
                    if (std::holds_alternative<int>(value) && std::is_same_v<T, double>) {
                        columnData.push_back(static_cast<T>(std::get<int>(value)));
                    }
              
                    else if (std::holds_alternative<double>(value) && std::is_same_v<T, int>) {
                        columnData.push_back(static_cast<T>(std::get<double>(value)));
                    }
                   
                }
                
                else if constexpr (std::is_same_v<T, std::string>) {
                    if (std::holds_alternative<std::string>(value)) {
                        columnData.push_back(std::get<std::string>(value));
                    }
                }
            } 
            catch (const std::bad_variant_access&) {
                continue;
            }
        }
    }

    if (columnData.empty()) {
        throw std::runtime_error(
            "No valid data of requested type found in column '" + columnName + "'");
    }

    return columnData;
}

} // namespace ScientificToolbox::Utils

#endif // UTILS_HPP
