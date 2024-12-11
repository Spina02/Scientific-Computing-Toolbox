#ifndef IMPORTCSV_HPP
#define IMPORTCSV_HPP


#include "ImportData.hpp"
#include "../Interpolation_Module/utilities_interpolation.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <optional>
#include <variant>
#include <set>
#include <type_traits>
#include <concepts>
#include <stdexcept>
#include <iostream>
#include <charconv>

const inline bool DEBUG = false;

using DataValue = std::variant<int, double, std::string>;
using OptionalDataValue = std::optional<DataValue>;

/**
 * @class ImportCSV
 * @brief Class for importing and parsing CSV files with mixed data types
 * @inherits ImportData
 * 
 * This class provides functionality to read CSV files and parse their contents into
 * a data structure that can handle mixed types (integers, doubles, strings) and null values.
 */

/**
 * @method import
 * @brief Main method to import data from a CSV file
 * @param filename The path to the CSV file to be imported
 * @throws std::runtime_error if the file cannot be opened
 * 
 * Opens and reads a CSV file line by line, parsing the header and subsequent data rows.
 */

/** @method read_points_from_csv
 * @brief Reads a CSV file and returns a set of points
 * @tparam T The data type of the points
 * @param filename The path to the CSV file
 * @return Set of points read from the CSV file
 * 
 * This method reads a CSV file containing x,y pairs of data and returns a set of points
 * of type T. The CSV file is expected to have a format where each line contains two values
 * separated by a comma, representing the x and y coordinates of a point. The points are
 * stored in a set to ensure uniqueness and sorted order. The sorted order is important here since 
 * the points are used for interpolation, and the interpolation methods require the points to be sorted.
 * Also, the set ensures that there are no duplicate points in the data.
 */

/**
 * @method parseHeader
 * @brief Parses the header line of the CSV file
 * @param line String containing the header line
 * @private
 * 
 * Splits the header line by commas and stores column names in headers_ vector.
 */

/**
 * @method parseLine
 * @brief Parses a single data line from the CSV file
 * @param line String containing the data line
 * @private
 * 
 * Processes each cell in the data line, handling empty values as null
 * and converting non-empty values to appropriate data types.
 * Stores the parsed data in the data_ structure.
 */

/**
 * @method parseValue
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

/**
 * @var headers_
 * @brief Vector storing the column names from the CSV file
 * @private
 * 
 * 
 * @note variables enending with _ are private member variables  https://stackoverflow.com/questions/3650623/trailing-underscores-for-member-variables-in-c
 */

/** namespace ScientificToolbox
 * @brief Namespace containing classes and functions for scientific computing
 * 
 * This namespace contains a collection of classes and functions for scientific computing,
 * including statistical analysis, data manipulation, and numerical methods.
 * 
 */
namespace ScientificToolbox {

class ImportCSV : public ImportData{
public: 
    void import(const std::string& filename) override{
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
    
    // Function to read CSV file and output std::set<point<T>> for double values
    template <typename T>
    std::set<ScientificToolbox::Interpolation::point<T>> read_points_from_csv(const std::string& filename) {
        std::set<ScientificToolbox::Interpolation::point<T>> points;
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Could not open the file!");
        }

        std::string line;

        // Skip the first line (header)
        std::getline(file, line);
        if (DEBUG)
            // Debugging header
            std::cout << "Skipped header: " << line << std::endl;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            T x, y;
            char comma;

            if (DEBUG)
                // Debugging: Print the line being read
                std::cout << "Reading line: " << line << std::endl;

            if (ss >> x >> comma >> y) {
                if (DEBUG)
                    // Debugging: Print parsed values
                    std::cout << "Parsed values: x = " << x << ", y = " << y << std::endl;
                points.insert(ScientificToolbox::Interpolation::point<T>(x, y));
            } else {
                std::cerr << "Failed to parse line: " << line << std::endl;
                throw std::runtime_error("Failed to parse numeric values from the CSV file. Check the file content, it should not contain characters or strings but only two column with x and y values respectively.");
            }

            // printing the points
            if (DEBUG) {
                std::cout << "Points: ";
                for (const auto& p : points) {
                    std::cout << "(" << p.get_x() << ", " << p.get_y() << ") ";
                }
                std::cout << std::endl;
            }
        }

        file.close();
        return points;
    }


private: 
    std::vector<std::string> headers_;

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

    OptionalDataValue parseValue(const std::string& cell) {
        std::string trimmed_cell = trim(cell);

        if (trimmed_cell.find(',') != std::string::npos) {
            return trimmed_cell;
        }

        int int_value;
        auto int_result = std::from_chars(trimmed_cell.data(), trimmed_cell.data() + trimmed_cell.size(), int_value);
        if (int_result.ec == std::errc() && int_result.ptr == trimmed_cell.data() + trimmed_cell.size()) {
            return int_value;
        }

        double double_value;
        try {
            double_value = std::stod(trimmed_cell);
            return double_value;
        } catch (...) {
            return trimmed_cell;
        }
    }
};
}

#endif