//
//  Calculator.hpp
//  COP3503_Project
//
//  Created by Ben on 8/3/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#ifndef Calculator_cpp
#define Calculator_cpp

#include "Expression.hpp"
#include <vector>
#include <string>
#include <deque>

class Calculator {
    
private:
    std::deque<Expression*> prevAnswers;
    int comparePrecedence(std::string lhs,std::string rhs);
    bool isOperation(std::string operation);
    bool isParenthesis(std::string token);
    std::vector<std::string> convertToRPN(std::vector<std::string> tokens);
    std::vector<std::string> tokenizer(const std::string& rawString);
    Expression* treeBuilder(std::vector<std::string> RPNtokens);
public:
    void solve(std::string input);
    ~Calculator();
};

#endif /* Calculator_cpp */
