#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <unordered_map>
#include <optional>
#include <variant>
#include <stdexcept>
#include <iostream>

using DataValue = std::variant<int, double, std::string>;
using OptionalDataValue = std::optional<DataValue>;
/**
 * @brief Extracts a column of specified type from a dataset structured as a vector of maps
 * 
 * @details This template function processes a dataset where each row is represented as an 
 * unordered map with string keys and OptionalDataValue values. It extracts values of type T 
 * from a specified column, with support for type conversion between numeric types.
 * 
 * Features:
 * - Handles missing values (OptionalDataValue)
 * - Supports type conversion between compatible numeric types (int to double, double to int)
 * - Performs validation checks on data structure and content
 * 
 * @tparam T The desired output type for the extracted column data
 * 
 * @param data Vector of unordered maps representing the dataset, where each map is a row
 * @param columnName String identifier for the column to extract
 * 
 * @return std::vector<T> A vector containing the extracted column values of type T
 * 
 * @throws std::runtime_error if:
 *     - Input data vector is empty
 *     - Specified column name doesn't exist in the dataset
 *     - No valid data of requested type is found in the column
 * 
 * @note The function will skip invalid or incompatible values without throwing an exception,
 *       but will throw if no valid values are found at all.
 * 
 * @example
 *     // Extract a column of doubles
 *     std::vector<double> numericColumn = extractColumn<double>(dataset, "price");
 *     
 *     // Extract a column of integers
 *     std::vector<int> integerColumn = extractColumn<int>(dataset, "quantity");
 */
namespace StatsModule::Utils{

template <typename T>
std::vector<T> extractColumn(const std::vector<std::unordered_map<std::string, OptionalDataValue>>& data,
                             const std::string& columnName) {
    if (data.empty()) {
        throw std::runtime_error("Data is empty");
    }

  
    if (data[0].find(columnName) == data[0].end()) {
        throw std::runtime_error("Column '" + columnName + "' does not exist");
    }

    std::vector<T> columnData;
    for (const auto& row : data) {
        auto it = row.find(columnName);
        if (it != row.end() && it->second.has_value()) {
            try {
                auto& value = it->second.value();
                if (std::holds_alternative<T>(value)) {
                    columnData.push_back(std::get<T>(value));
                } else if constexpr (std::is_arithmetic_v<T>) {
                    
                    if (std::holds_alternative<int>(value) && std::is_same_v<T, double>) {
                        columnData.push_back(static_cast<T>(std::get<int>(value)));
                    } else if (std::holds_alternative<double>(value) && std::is_same_v<T, int>) {
                        columnData.push_back(static_cast<T>(std::get<double>(value)));
                    }
                }
            } catch (const std::bad_variant_access&) {
                continue;
            }
        }
    }
    if (columnData.empty()) {
        throw std::runtime_error(" No valid data of requested type found in column '" + columnName + "'");
    }

    

    return columnData;
}
}    
#endif