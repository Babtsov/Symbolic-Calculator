//
//  Tokenizer.hpp
//  COP3503_Project
//
//  Created by Ben on 7/20/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#ifndef Tokenizer_cpp
#define Tokenizer_cpp

#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <stack>
#include <vector>
#include <math.h>


bool isOperation(std::string operation);
bool isParenthesis(std::string token);
int comparePrecedence(std::string lhs,std::string rhs);
std::vector<std::string> convertToRPN(std::vector<std::string> tokens);
std::vector<std::string> tokenizer(const std::string& rawString);

#endif /* Tokenizer_cpp */
