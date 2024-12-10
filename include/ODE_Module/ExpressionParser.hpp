#ifndef EXPRESSION_PARSER_HPP
#define EXPRESSION_PARSER_HPP

#include "utils.hpp"

namespace ScientificToolbox::ODE {
    var_func parseExpression(const var_expr& expr);
    scalar_func parseScalarExpression(const std::string &expr);
    vec_func parseVectorExpression(const vec_s &exprs);
}

#endif // EXPRESSION_PARSER_HPP