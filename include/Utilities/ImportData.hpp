#ifndef IMPORTDATA_HPP
#define IMPORTDATA_HPP


#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>


using DataValue = std::variant<int, double, std::string>;
using OptionalDataValue = std::optional<DataValue>;

/** namespace ScientificToolbox
 * @brief Namespace containing classes and functions for scientific computing
 * 
 * This namespace contains a collection of classes and functions for scientific computing,
 * in particulare it contains 3 modules: ODE_Module, Interpolation_Module and Utilities.
 * 
 */
namespace ScientificToolbox{

/**
 * @class ImportData
 * @brief Abstract base class defining interface for data import operations
 * 
 * This abstract class provides a common interface for importing data from various
 * sources into a standardized internal format. It uses a vector of unordered maps
 * to store data, where each map represents a row/entry with string keys and
 * OptionalDataValue values.
 * 
 * This abstract class provides the following features:
 * - Virtual destructor for proper cleanup in derived classes
 * - Pure virtual import function for implementing specific import logic
 * - Protected data storage accessible to derived classes
 * - Const getter method for safe data access
 * 
 * @note Derived classes must implement the import() method according to their
 * specific data source requirements
 * 
 * @see OptionalDataValue
 */
class ImportData{

public:
    virtual ~ImportData() = default;

    virtual void import(const std::string& filename) = 0;

    /**
     * @brief Retrieves the stored data from the object
     * 
     * @return A const reference to a vector of unordered maps, where each map contains
     *         string keys paired with OptionalDataValue objects representing the imported data
     */
    const std::vector<std::unordered_map<std::string, OptionalDataValue>>& getData() const{
        return data_;
    }

protected:
    std::vector<std::unordered_map<std::string, OptionalDataValue>> data_;
};

}

#endif



