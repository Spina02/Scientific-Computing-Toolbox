#ifndef EXPRESSION_PARSER_HPP
#define EXPRESSION_PARSER_HPP

#include "utils.hpp"

namespace ScientificToolbox::ODE {
    scalar_func parseScalarExpression(const std::string &expr);
    vec_func parseVectorExpression(const vec_s &exprs);
    var_func parseExpression(const var_expr& expr);
}

#endif // EXPRESSION_PARSER_HPP