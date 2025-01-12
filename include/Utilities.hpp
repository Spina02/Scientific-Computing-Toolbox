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
 * @class ImportCSV
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
    void import(const std::string& filename);

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
    void parseHeader(const std::string& line);

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
    void parseLine(const std::string& line);
    
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
    std::string trim(const std::string& str);

    /**
     *  parseValue
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
    OptionalDataValue parseValue(const std::string& cell);
};
}

#endif // UTILITIES_HPP