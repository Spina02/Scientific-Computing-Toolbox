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

using DataValue = std::variant<int, double, std::string>;
using OptionalDataValue = std::optional<DataValue>;

/** namespace ScientificToolbox
 * @brief Namespace containing classes and functions for scientific computing
 * 
 * This namespace contains a collection of classes and functions for scientific computing,
 * including statistical analysis, data manipulation, and numerical methods.
 * 
 */

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
 * separated by a comma, representing the x and y coordinates of a point.
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
    std::set<point<T>> read_points_from_csv(const std::string& filename) {
        std::set<point<T>> points;
        std::ifstream file(filename);
        
        // Check if the file is open
        if (!file.is_open()) {
            throw std::runtime_error("Could not open the file!");
            return points;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            T x, y;

            // Read x and y from each line (assuming CSV format "x,y")
            char comma;
            if (ss >> x >> comma >> y) {
                points.insert(point<T>(x, y));
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

        while (std::getline(ss,header, ',')) {
            headers_.push_back(header);
        }


    }

    void parseLine(const std::string& line){

        std::stringstream ss(line);
        std::string cell;
        std::unordered_map<std::string, OptionalDataValue> row;

        size_t i = 0;

        while (std::getline(ss,cell, ',')) {

            if (cell.empty()) {

                row[headers_[i]] = std::nullopt;

            } else {

                row[headers_[i]] = parseValue(cell);

            }

            i++;
        }

        data_.push_back(row);

    }


    OptionalDataValue parseValue(const std::string& cell) {

        try {
            int int_value = std::stoi(cell);
            return int_value;
        } catch (...){}

        try {
            double double_value = std::stod(cell);
            return double_value; 
        } catch (...){}

        return cell;
    }


};
}

#endif