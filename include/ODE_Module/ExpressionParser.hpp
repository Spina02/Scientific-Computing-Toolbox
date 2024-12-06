#ifndef EXPRESSION_PARSER_HPP
#define EXPRESSION_PARSER_HPP

#include "../../include/Utilities.hpp"

namespace ScientificToolbox {
    scalar_func parseScalarExpression(const std::string &expr);
    vec_func parseVectorExpression(const vec_s &exprs);
}

#endif // EXPRESSION_PARSER_HPP