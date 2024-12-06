#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <vector>
#include <functional>
#include "muParser.h"

namespace ScientificToolbox {
    // type aliases
    //typedef std::vector<int> vec_i;
    using vec_i = std::vector<int>;
    using vec_d = std::vector<double>;
    using vec_s = std::vector<std::string>;
    using scalar_func = std::function<double(double, double)>;
    using vec_func = std::function<vec_d(double, vec_d)>;
    
    // function declarations
}

#endif // UTILITIES_HPP