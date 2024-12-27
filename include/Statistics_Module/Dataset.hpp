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

private:
    std::vector<std::unordered_map<std::string, OptionalDataValue>> data;




};
}

#endif // DATASET_HPP