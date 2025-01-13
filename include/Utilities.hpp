#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <optional>
#include <variant>
#include <set>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <chrono>

const inline bool DEBUG = false;
using DataValue = std::variant<int, double, std::string>;
using OptionalDataValue = std::optional<DataValue>;

/** namespace ScientificToolbox
 * @brief Namespace containing classes and functions for scientific computing
 * 
 * This namespace contains a collection of classes and functions for scientific computing,
 * including statistical analysis, data manipulation, and numerical methods.
 * 
 */
namespace ScientificToolbox {

template <typename T, typename Callable, typename... Args>
T measure_execution_time(Callable&& callback, Args&&... args) {
    
    auto start = std::chrono::high_resolution_clock::now();
    T result = std::forward<Callable>(callback)(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
    
    return result;
}

template <typename T>
T measure_execution_time_py(std::function<T()> callback) {
    return measure_execution_time<T>(callback);
}

/**
 * @brief 
 * 
 * @class Importer
 * @brief Class for importing and parsing CSV files with mixed data types
 * 
 * This class provides functionality to read CSV files and parse their contents into
 * a data structure that can handle mixed types (integers, doubles, strings) and null values.
 * It uses a vector of unordered maps to store data, where each map represents a row/entry 
 * with string keys and OptionalDataValue values.
 * 
 * Key features:
 * - Import and parse CSV files with mixed data types
 * - Handle null/empty values
 * - Store data in a standardized internal format
 * - Provide safe data access methods
 * 
 * @see OptionalDataValue
 */

class Importer {
public: 

    /**
     *  import
     * @brief Main method to import data from a CSV file
     * @param filename The path to the CSV file to be imported
     * @throws std::runtime_error if the file cannot be opened
     * 
     * Opens and reads a CSV file line by line, parsing the header and subsequent data rows.
     */
    void import(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open CSV file.");
        }

        std::string line;
        std::getline(file, line);

        parseHeader(line);

        while (std::getline(file, line)) {
            parseLine(line);
        }
    }

    /**
     * @brief Retrieves the stored data from the object
     * 
     * @return A const reference to a vector of unordered maps, where each map contains
     *         string keys paired with OptionalDataValue objects representing the imported data
     */
    const std::vector<std::unordered_map<std::string, OptionalDataValue>>& getData() const{
        return data_;
    }

private:
    std::vector<std::unordered_map<std::string, OptionalDataValue>> data_;
    std::vector<std::string> headers_;

    /**
     *  parseHeader
     * @brief Parses the header line of the CSV file
     * @param line String containing the header line
     * @private
     * 
     * Splits the header line by commas and stores column names in headers_ vector.
     */
    void parseHeader(const std::string& line) {
        std::stringstream ss(line);
        std::string header;

        // Clear existing headers before parsing new ones
        headers_.clear();

        // Parse headers from the first line
        while (std::getline(ss, header, ',')) {
            if (!header.empty()) {
                headers_.push_back(header);
            } else {
                std::cerr << "Warning: Empty header found" << std::endl;
            }
        }

        // Check if any headers were found
        if (headers_.empty()) {
            throw std::runtime_error("No headers found in the CSV file.");
        }

        // Debugging: Print parsed headers
        if (DEBUG) {
            std::cout << "Parsed headers: ";
            for (const auto& h : headers_) {
                std::cout << h << " ";
            }
            std::cout << std::endl;
        }
    }

    /**
     * parseLine
     * @brief Parses a single data line from the CSV file
     * @param line String containing the data line
     * @private
     * 
     * Processes each cell in the data line, handling empty values as null
     * and converting non-empty values to appropriate data types.
     * Stores the parsed data in the data_ structure.
     */
    void parseLine(const std::string& line) {
        std::stringstream ss(line);
        std::string cell;
        std::unordered_map<std::string, OptionalDataValue> row;

        size_t i = 0;
        bool inside_quotes = false;
        std::string current_cell;

        // Parse each cell in the line
        for (char ch : line) {
            // Handle quotes and commas
            if (ch == '"') {
                inside_quotes = !inside_quotes;
            } 
            // 
            else if (ch == ',' && !inside_quotes) {
                if (i >= headers_.size()) {
                    std::cerr << "Warning: More cells than headers in line: " << line << std::endl;
                    break;
                }

                if (current_cell.empty()) {
                    row[headers_[i]] = std::nullopt;
                } else {
                    row[headers_[i]] = parseValue(current_cell);
                }
                current_cell.clear();
                i++;
            } else {
                current_cell += ch;
            }
        }

        if (!current_cell.empty()) {
            if (i >= headers_.size()) {
                std::cerr << "Warning: More cells than headers in line: " << line << std::endl;
            } else {
                row[headers_[i]] = parseValue(current_cell);
            }
        }

        if (i < headers_.size() - 1) {
            std::cerr << "Warning: Fewer cells ("<< i <<") than headers in line ("<< headers_.size() <<"): " << std::endl; // line << std::endl;
        }

        data_.push_back(row);
    }
    
    /**
     * @brief Removes leading and trailing whitespace and quotes from a string
     * 
     * This function trims whitespace from both ends of a string and optionally removes
     * enclosing quotation marks if present.
     * 
     * @param str The input string to be trimmed
     * @return std::string The trimmed string with whitespace and optional quotes removed
     * 
     * @details The function:
     * 1. Removes leading whitespace characters
     * 2. Removes trailing whitespace characters  
     * 3. If the remaining string starts and ends with quotes, removes them
     */
    std::string trim(const std::string& str) {
        size_t start = 0;
        while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start]))) {
            ++start;
        }

        size_t end = str.size();
        while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
            --end;
        }

        // Remove quotes if present
        if (start < end && str[start] == '"' && str[end - 1] == '"') {
            ++start;
            --end;
        }

        return str.substr(start, end - start);
    }

    /**
     * @brief Attempts to parse a string value into an appropriate data type
     * @param cell String containing the value to parse
     * @return OptionalDataValue containing the parsed value
     * @private
     * 
     * Tries to convert the input string to:
     * 1. Integer
     * 2. Double 
     * If both conversions fail, returns the original string
     */
    OptionalDataValue parseValue(const std::string& cell) {
        std::string trimmed_cell = trim(cell);

        // Handle empty or comma-containing cells
        if (trimmed_cell.empty() || trimmed_cell.find(',') != std::string::npos) {
            return std::make_optional<DataValue>(trimmed_cell);
        }

        // Try parsing as integer
        try {
            size_t pos = 0;
            int int_value = std::stoi(trimmed_cell, &pos);
            if (pos == trimmed_cell.length()) {
                return std::make_optional<DataValue>(int_value);
            }
        } catch (...) {
            // Failed to parse as integer, continue to double
        }

        // Try parsing as double
        try {
            size_t pos = 0;
            double double_value = std::stod(trimmed_cell, &pos);
            if (pos == trimmed_cell.length()) {
                return std::make_optional<DataValue>(double_value);
            }
        } catch (...) {
            // Failed to parse as double, return as string
        }

        // Return as string if all numeric conversions fail
        return std::make_optional<DataValue>(trimmed_cell);
    }
};

} // namespace ScientificToolbox

#endif // UTILITIES_HPP