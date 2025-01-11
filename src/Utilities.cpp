#include "../include/Utilities.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <optional>
#include <variant>
#include <stdexcept>
#include <iostream>
#include <charconv>

namespace ScientificToolbox {

void Importer::import(const std::string& filename) {
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

void Importer::parseHeader(const std::string& line) {
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

void Importer::parseLine(const std::string& line) {
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

std::string Importer::trim(const std::string& str) {
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

OptionalDataValue Importer::parseValue(const std::string& cell) {
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

} // namespace ScientificToolbox