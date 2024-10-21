/*
 * @Date: 2024-09-30 14:11:29
 * @Author: DarkskyX15
 * @LastEditTime: 2024-10-12 15:43:53
 */
#include "Expression.hpp"
#include <string>
#include <iostream>

int main() {
    std::string expr;

    try {
        std::cout << "输入中缀表达式：\n>>> ";
        std::getline(std::cin, expr);
        tokens tk_list(splitTokens(expr));
        std::cout << normalExpressionEval(tk_list) << '\n';
    } catch (ExpressionError& e) {
        std::cout << "中缀表达式计算失败: " << e.what() << '\n' ;
    }

    // try {
    //     std::cout << "输入后缀表达式：\n>>> ";
    //     std::getline(std::cin, expr);
    //     tokens tk_list(readTokens(expr));
    //     std::cout << suffixExpressionEval(tk_list);
    // } catch (ExpressionError& e) {
    //     std::cout << "后缀表达式计算失败: " << e.what() << '\n' ;
    // }

    return 0;
}