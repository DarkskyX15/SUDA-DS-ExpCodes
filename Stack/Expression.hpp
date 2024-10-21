/*
 * @Date: 2024-10-11 21:27:23
 * @Author: DarkskyX15
 * @LastEditTime: 2024-10-14 15:08:10
 */

#ifndef _EXPRESSION_HPP_
#define _EXPRESSION_HPP_

#define IS_OPERATOR(chr) chr=='+'||chr=='-'||chr=='*'||chr=='/'
#define IS_BRACKET(chr) chr=='('||chr==')'

#include "Stack.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <exception>

using tokens = std::vector<std::string>;

class ExpressionError: public std::exception {
private:
    const char* err;
public:
    ExpressionError(const char* detail): err(detail) { }
    const char* what() const noexcept override { return err; }
};

static unsigned short priority[] = {0, 0, 1, 1, 10, 10};

enum class OperatorType: unsigned short { add, minus, mul, div, lb, rb, invalid };
OperatorType judgeOperator(char chr) {
    switch (chr) {
        case '+': return OperatorType::add;
        case '-': return OperatorType::minus;
        case '*': return OperatorType::mul;
        case '/': return OperatorType::div;
        case '(': return OperatorType::lb;
        case ')': return OperatorType::rb;
        default: return OperatorType::invalid;
    }
}

tokens readTokens(std::string whitespace_splitted) {
    std::stringstream rd(whitespace_splitted, std::ios::in);
    std::string token;
    tokens token_list;
    while (rd >> token) { token_list.push_back(token); }
    return token_list;
}

tokens splitTokens(std::string expression) {
    tokens token_list;
    std::string buffer;
    size_t index = 0;
    while (index < expression.size()) {
        char chr = expression[index];
        if (IS_OPERATOR(chr) || IS_BRACKET(chr)) {
            if (buffer.size()) {
                token_list.push_back(buffer);
                buffer.clear();
            }
            buffer.push_back(chr);
            token_list.push_back(buffer);
            buffer.clear();
        } else {
            if (('0' <= chr && chr <= '9') || chr == '.') {
                buffer.push_back(chr);
            } else {
                throw ExpressionError("Invalid characters.");
            }
        }
        ++index;
    }
    if (buffer.size()) token_list.push_back(buffer);
    return token_list;
}

double suffixExpressionEval(const tokens& token_list) {
    Stack<double> st;
    for (const auto& token: token_list) {
        OperatorType ot = judgeOperator(token[0]);
        if (ot != OperatorType::invalid) {
            if (st.size() < 2) { throw ExpressionError("Incorrect expression."); }
            double cd1, cd2;
            cd2 = st.pop();
            cd1 = st.pop();
            if (ot == OperatorType::add) { st.push(cd1 + cd2); }
            else if (ot == OperatorType::minus) { st.push(cd1 - cd2); }
            else if (ot == OperatorType::mul) { st.push(cd1 * cd2); }
            else if (ot == OperatorType::div) { st.push(cd1 / cd2); }
        } else {
            try {
                st.push(std::stod(token));
            } catch (std::exception& e) {
                throw ExpressionError("Invalid token.");
            }
        }
    }
    if (st.size() > 1) { throw ExpressionError("Incorrect expression."); }
    return st.pop();
}

double normalExpressionEval(const tokens& token_list) {
    Stack<double> number_stack;
    Stack<OperatorType> operator_stack;
    for (const auto& token: token_list) {
        OperatorType ot = judgeOperator(token[0]);
        if (ot != OperatorType::invalid) {
            if (ot == OperatorType::lb) {
                operator_stack.push(ot);
            } else if (ot == OperatorType::rb) {
                bool norm_exit = false;
                while (!operator_stack.empty()) {
                    if (operator_stack.top() == OperatorType::lb) {
                        operator_stack.pop();
                        norm_exit = true;
                        break;
                    } else {
                        auto op = operator_stack.pop();
                        if (number_stack.size() < 2) {
                            throw ExpressionError("Incorrect expression.");
                        }
                        double cd1, cd2;
                        cd2 = number_stack.pop();
                        cd1 = number_stack.pop();
                        if (op == OperatorType::add) {number_stack.push(cd1 + cd2);}
                        else if (op == OperatorType::minus) {number_stack.push(cd1 - cd2);}
                        else if (op == OperatorType::mul) {number_stack.push(cd1 * cd2);}
                        else if (op == OperatorType::div) {number_stack.push(cd1 / cd2);}
                    }
                }
                if (!norm_exit) throw ExpressionError("Unmatched brackets.");
            } else {
                while (
                    operator_stack.size() &&
                    operator_stack.const_top() != OperatorType::lb &&
                    priority[(int)ot] <= priority[(int)operator_stack.const_top()]) {
                    
                    auto op = operator_stack.pop();
                    if (number_stack.size() < 2) {
                        throw ExpressionError("Incorrect expression.");
                    }
                    double cd1, cd2;
                    cd2 = number_stack.pop();
                    cd1 = number_stack.pop();
                    if (op == OperatorType::add) {number_stack.push(cd1 + cd2);}
                    else if (op == OperatorType::minus) {number_stack.push(cd1 - cd2);}
                    else if (op == OperatorType::mul) {number_stack.push(cd1 * cd2);}
                    else if (op == OperatorType::div) {number_stack.push(cd1 / cd2);}
                }
                operator_stack.push(ot);
            }
        } else {
            try {
                number_stack.push(std::stod(token));
            } catch (std::exception& e) {
                throw ExpressionError("Invalid token.");
            }
        }
    }
    while (!operator_stack.empty()) {
        auto op = operator_stack.pop();
        if (op == OperatorType::lb) {
            throw ExpressionError("Unmatched brackets.");
        }
        if (number_stack.size() < 2) {
            throw ExpressionError("Incorrect expression.");
        }
        double cd1, cd2;
        cd2 = number_stack.pop();
        cd1 = number_stack.pop();
        if (op == OperatorType::add) {number_stack.push(cd1 + cd2);}
        else if (op == OperatorType::minus) {number_stack.push(cd1 - cd2);}
        else if (op == OperatorType::mul) {number_stack.push(cd1 * cd2);}
        else if (op == OperatorType::div) {number_stack.push(cd1 / cd2);}
    }
    if (number_stack.size() > 1) { throw ExpressionError("Incorrect expression."); }
    return number_stack.pop();
}

#endif /* _EXPRESSION_HPP_ */
