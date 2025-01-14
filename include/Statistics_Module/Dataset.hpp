#ifndef DATASET_HPP
#define DATASET_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include <optional>
#include <variant>
#include "Utils.hpp"

namespace ScientificToolbox::Statistics {

/**
 * @brief A class representing a dataset for statistical analysis
 * 
 * The Dataset class provides functionality to store and manipulate tabular data
 * where each row is a collection of named values, and each value can be of different types
 * (handled through OptionalDataValue). The data structure supports mixed-type columns
 * and missing values.
 * 
 * @details The class implements:
 * - Forward iterator support for row-wise traversal
 * - Column-based data access
 * - Dynamic row addition
 * - Column type checking
 * 
 * The internal structure uses a vector of unordered maps, where:
 * - Each vector element represents a row
 * - Each unordered_map represents columns as key-value pairs
 * - Keys are column names (strings)
 * - Values are OptionalDataValue objects that can hold different types
 * 
 * Iterator implementation provides standard forward iterator capabilities
 * conforming to C++ iterator requirements.
 * 
 * @note The class is part of the ScientificToolbox::Statistics namespace
 * and is designed for scientific computing applications.
 * 
 * Usage example:
 * @code
 * Dataset ds;
 * ds.addRow({{"col1", 1}, {"col2", "value"}});
 * auto numericData = ds.getColumn<double>("col1");
 * @endcode
 */
class Dataset {
public:
    class Iterator {

        private:
            std::vector<std::unordered_map<std::string, OptionalDataValue>>::iterator it;
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::unordered_map<std::string, OptionalDataValue>;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type*;
            using reference = value_type&;

            Iterator(std::vector<std::unordered_map<std::string, OptionalDataValue>>::iterator iter): it(iter) {}

            reference operator*()  {
                return *it;
            }
            pointer operator->() {
                return &(*it);
            }
            Iterator& operator++() {
                ++it;
                return *this;
            }
            Iterator operator++(int) {
                Iterator tmp = *this;
                ++it;
                return tmp;
            }

            bool operator==(const Iterator& other) const {
                return it == other.it;
            }

            bool operator!=(const Iterator& other) const {
                return it != other.it;
            }


    };

    Dataset() = default;
    explicit Dataset(const std::vector<std::unordered_map<std::string, OptionalDataValue>>& data);

    //methods

    Iterator begin() {
        return Iterator(data.begin());
    }
    Iterator end() {
        return Iterator(data.end());
    }

    template <typename T>
    std::vector<T> getColumn(const std::string& columnName) const;
    std::vector<std::string> getColumnNames() const;
    size_t size() const {return data.size();}
    bool empty() const {return data.empty();}

    void addRow(const std::unordered_map<std::string, OptionalDataValue>& row);


    bool isNumericColumn(const std::string& columnName) const;

    


private:
    std::vector<std::unordered_map<std::string, OptionalDataValue>> data;




};
}

#endif // DATASET_HPP