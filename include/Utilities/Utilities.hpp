#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <vector>
#include <functional>
#include <variant>
#include <Eigen/Dense>

namespace ScientificToolbox {
    using scalar_func = std::function<double(double, double)>;
    using vec_func = std::function<Eigen::VectorXd(double, const Eigen::VectorXd&)>;
    using var_func = std::variant<scalar_func, vec_func>;
    using var_vec = std::variant<double, Eigen::VectorXd>;
    
    using vec_d = Eigen::VectorXd;
    using vec_s = std::vector<std::string>;
    using var_vecs = std::vector<var_vec>;
    using var_expr = std::variant<std::string, vec_s>;

    inline const bool DEBUG = true;
}

#endif // UTILITIES_HPP