#include "../../include/ODE_Module/ExpressionParser.hpp"
#include <muParser.h>
#include <memory>

namespace ScientificToolbox::ODE {
    scalar_func parseScalarExpression(const std::string& expr) {
        try {
            if (expr.empty()) {
                throw std::runtime_error("The expression is empty.");
            }
            // shared pointer for `t` and `y`
            auto parser = std::make_shared<mu::Parser>();
            auto t_ptr = std::make_shared<double>(0.0);
            auto y_ptr = std::make_shared<double>(0.0);

            // initialize parser
            parser->DefineVar("t", t_ptr.get());
            parser->DefineVar("y", y_ptr.get());
            parser->SetExpr(expr);

            // Lambda function that evaluates the expression
            return [parser, t_ptr, y_ptr](double t, double y) -> double {
                *t_ptr = t;
                *y_ptr = y;
                return parser->Eval();
            };
        } catch (const mu::Parser::exception_type &e) {
            throw std::runtime_error("Error parsing scalar expression: " + std::string(e.GetMsg()));
        }
    }

    vec_func parseVectorExpression(const vec_s& exprs) {
        try {
            // shared pointer for `t` and for each y
            auto t_ptr = std::make_shared<double>(0.0);
            auto y_ptrs = std::make_shared<std::vector<std::shared_ptr<double>>>();
            y_ptrs->resize(exprs.size());

            // parser vector for each expression
            std::vector<std::shared_ptr<mu::Parser>> parsers(exprs.size());

            // initialize parsers
            for (size_t i = 0; i < exprs.size(); ++i) {
                if (exprs[i].empty()) {
                    throw std::runtime_error("Expression " + std::to_string(i) + " is empty.");
                }

                parsers[i] = std::make_shared<mu::Parser>();
                parsers[i]->DefineVar("t", t_ptr.get());

                // For single component systems, allow using just 'y'
                if (exprs.size() == 1) {
                    if (!y_ptrs->at(0)) {
                        y_ptrs->at(0) = std::make_shared<double>(0.0);
                    }
                    parsers[i]->DefineVar("y", y_ptrs->at(0).get());
                }

                // Always define y0,y1,... style variables
                for (size_t j = 0; j < exprs.size(); ++j) {
                    if (!y_ptrs->at(j)) {
                        y_ptrs->at(j) = std::make_shared<double>(0.0);
                    }
                    parsers[i]->DefineVar("y" + std::to_string(j), y_ptrs->at(j).get());
                }
                parsers[i]->SetExpr(exprs[i]);
            }

            // Lambda function that evaluates the expressions
            return [parsers, t_ptr, y_ptrs](double t, const vec_d &y) -> vec_d {
                if (static_cast<size_t>(y.size()) != y_ptrs->size()) {
                    throw std::runtime_error("Mismatch between number of expressions and size of y vector.");
                }

                // Update the value of `t`
                *t_ptr = t;

                // Update the value of each `y`
                for (Eigen::Index i = 0; i < y.size(); ++i) {
                    *(y_ptrs->at(i)) = y[i];
                }

                // Evaluate each expression
                vec_d result(parsers.size());
                for (size_t i = 0; i < parsers.size(); ++i) {
                    result(i) = parsers[i]->Eval();
                }

                return result;
            };
        } catch (const mu::Parser::exception_type& e) {
            throw std::runtime_error("Error parsing vector expression: " + std::string(e.GetMsg()));
        }
    }

    var_func parseExpression(const var_expr& expr) {
        try {
            if (std::holds_alternative<std::string>(expr)) {
                return parseScalarExpression(std::get<std::string>(expr));
            } else if (std::holds_alternative<vec_s>(expr)) {
                return parseVectorExpression(std::get<vec_s>(expr));
            } else {
                throw std::runtime_error("Invalid expression type.");
            }
        } catch (const std::exception& e) {
            throw std::runtime_error("Error parsing expression: " + std::string(e.what()));
        }
    }

}

