#ifndef ODE_TYPES_HPP
#define ODE_TYPES_HPP

#include <functional>
#include <variant>
#include <Eigen/Dense>
#include <vector>
#include <string>

namespace ScientificToolbox::ODE {

using scalar_func = std::function<double(double, double)>;
using vec_func = std::function<Eigen::VectorXd(double, const Eigen::VectorXd&)>;
using var_func = std::variant<scalar_func, vec_func>;
using var_vec = std::variant<double, Eigen::VectorXd>;

using vec_d = Eigen::VectorXd;
using vec_s = std::vector<std::string>;
using var_vecs = std::vector<var_vec>;
using var_expr = std::variant<std::string, vec_s>;

// Overload for printing variant types
std::ostream& operator<<(std::ostream& os, const var_vec& vec);
std::ostream& operator<<(std::ostream& os, const var_expr& expr);

// Overload for arithmetic operations
var_vec operator*(double h, const var_vec& v);
var_vec operator+(const var_vec& v1, const var_vec& v2);
var_vec operator-(const var_vec& v1, const var_vec& v2);
var_vec operator/(const var_vec& v1, const double v2);
var_vec operator/(const var_vec& v1, const var_vec& v2);

struct ODETestCase {
    var_expr expr;
    double t0;
    double tf;
    double h;
    var_vec y0;
    var_vec expected_final;
    var_vec expected_derivative;
};

struct Func {
    var_func func;
    var_vec operator()(double t, const var_vec& y) const {
        return std::visit([&](auto&& f) -> var_vec {
            using T = std::decay_t<decltype(f)>;
            if constexpr (std::is_same_v<T, scalar_func>) {
                double y_double = std::get<double>(y);
                return var_vec(f(t, y_double));
            } else {
                vec_d y_vec = std::get<vec_d>(y);
                return var_vec(f(t, y_vec));
            }
        }, func);
    }
};

template <typename Variant>
std::ostream& print_variant(std::ostream& os, const Variant& var) {
    std::visit([&os](const auto& value) {
        using ValueType = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<ValueType, Eigen::VectorXd>) {
            os << "[";
            for (int i = 0; i < value.size(); ++i) {
                os << value(i);
                if (i < value.size() - 1) {
                    os << ", ";
                }
            }
            os << "]";
        } else if constexpr (std::is_same_v<ValueType, std::vector<std::string>>) {
            if (value.empty()) {
                os << "<empty vector>";
            } else {
                os << "[";
                for (size_t i = 0; i < value.size(); ++i) {
                    os << value[i];
                    if (i < value.size() - 1) {
                        os << ", ";
                    }
                }
                os << "]";
            }
        } else {
            os << value;
        }
    }, var);
    return os;
}

template <typename Func>
var_vec apply_binary_operation(const var_vec& v1, const var_vec& v2, Func op) {
    if (std::holds_alternative<double>(v1)) {
        return op(std::get<double>(v1), std::get<double>(v2));
    } else {
        return op(std::get<Eigen::VectorXd>(v1), std::get<Eigen::VectorXd>(v2));
    }
}

template <typename Func>
var_vec apply_unary_operation(const var_vec& v, double scalar, Func op) {
    if (std::holds_alternative<double>(v)) {
        return op(std::get<double>(v), scalar);
    } else {
        return op(std::get<Eigen::VectorXd>(v), scalar);
    }
}

} // namespace ScientificToolbox::ODE

#endif // ODE_TYPES_HPP