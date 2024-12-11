#include "../../include/ODE_Module/types.hpp"

#include <iostream>

namespace ScientificToolbox::ODE {

std::ostream& operator<<(std::ostream& os, const var_vec& vec) {
    return print_variant(os, vec);
}

std::ostream& operator<<(std::ostream& os, const var_expr& expr) {
    return print_variant(os, expr);
}

std::ostream& operator<<(std::ostream& os, const ODESolution& solution) {
    size_t n = solution.size;
    os << "\n    Solution trajectory:" << std::endl;
    size_t step = std::max(1, static_cast<int>(n / solution.steps));
    for (size_t i = 0; i < n; i += step) {
        os << "    t = " << solution.t_values[i] << ", y = " << solution.y_values[i] << std::endl;
    }
    os << "    t = " << solution.t_values[n] << ", y = " << solution.y_values.back() << std::endl;
    return os;
}

var_vec operator*(double h, const var_vec& v) {
    return apply_unary_operation(v, h, [](const auto& value, double scalar) {
        return value * scalar;
    });
}

var_vec operator+(const var_vec& v1, const var_vec& v2) {
    return apply_binary_operation(v1, v2, [](const auto& a, const auto& b) {
        return a + b;
    });
}

var_vec operator-(const var_vec& v1, const var_vec& v2) {
    return apply_binary_operation(v1, v2, [](const auto& a, const auto& b) {
        return a - b;
    });
}

var_vec operator/(const var_vec& v1, const var_vec& v2) {
    return apply_binary_operation(v1, v2, [](const auto& a, const auto& b) {
        if constexpr (std::is_same_v<std::decay_t<decltype(a)>, Eigen::VectorXd>) {
            return a.cwiseQuotient(b);
        } else {
            return a / b;
        }
    });
}

var_vec operator/(const var_vec& v1, const double v2) {
    if (std::holds_alternative<double>(v1)) {
        return std::get<double>(v1) / v2;
    } else {
        return std::get<Eigen::VectorXd>(v1) / v2;
    }
}

} // namespace ScientificToolbox::ODE