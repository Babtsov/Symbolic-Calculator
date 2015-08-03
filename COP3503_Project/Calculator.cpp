//
//  Calculator.cpp
//  COP3503_Project
//
//  Created by Ben on 8/3/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Calculator.hpp"
#include "Addition.hpp"
#include "Multiplication.hpp"
#include "Division.hpp"
#include "Exponentiation.hpp"
#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <stack>
#include <vector>
#include <cmath>
#include <stdexcept>
using namespace std;

bool Calculator::isOperation(string operation){
    set<string> ops ={"+", "-", "*","/","rt","^"};
    return ops.count(operation) > 0;
}
bool Calculator::isParenthesis(string token) {
    return (token == "(" || token == ")");
}
int Calculator::comparePrecedence(string lhs,string rhs) {
    map<string,int> precedence = {{"+",0},{"-",0},{"*",1},{"/",1},{"rt",2},{"^",2}};
    return precedence[lhs] - precedence[rhs];
}
vector<string> Calculator::convertToRPN(vector<string> tokens) {
    vector<string> reversePolish;
    stack<string> opStack;
    for (auto itr = tokens.begin(); itr != tokens.end(); ++itr) {
        // if we have a variable, push it straight to the output
        if (!isOperation(*itr) && !isParenthesis(*itr)) {
            reversePolish.push_back(*itr);
            continue;
        }
        else if(isOperation(*itr)) {
            auto o1 = *itr;
            //while there is an operator token, o2, at the top of the operator stack
            while (!opStack.empty() && !isParenthesis(opStack.top()) ){
                auto o2 = opStack.top();
                // and either o1 is left-associative and its precedence is less than or equal to that of o2
                if (o1 != "^" && comparePrecedence(o1, o2) <= 0) {
                    // then pop o2 off the operator stack, onto the output queue
                    reversePolish.push_back(o2);
                    opStack.pop();
                }
                else
                    break;
            }
            // push o1 onto the operator stack.
            opStack.push(o1);
        }
        // If the token is a left parenthesis, then push it onto the stack.
        else if ( *itr == "(") {
            opStack.push(*itr);
        }
        /*
         If the token is a right parenthesis:
         Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue.
         Pop the left parenthesis from the stack, but not onto the output queue.
         If the stack runs out without finding a left parenthesis, then there are mismatched parentheses.
         */
        else if (*itr == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                reversePolish.push_back(opStack.top());
                opStack.pop();
            }
            if (opStack.empty() || opStack.top() != "(") {
                throw runtime_error("Mismatch in parenthesis. Please try again.\n");
            }
            opStack.pop();
        }
    }
    /*
     When there are no more tokens to read:
     While there are still operator tokens in the stack:
     If the operator token on the top of the stack is a parenthesis, then there are mismatched parentheses.
     Pop the operator onto the output queue.
     */
    while (!opStack.empty()) {
        if ( isParenthesis(opStack.top()) ) {
            throw runtime_error("Mismatch in parenthesis. Please try again.\n");
        }
        reversePolish.push_back(opStack.top());
        opStack.pop();
    }
    return reversePolish;
}
vector<string> Calculator::tokenizer(const string& rawString) {
    vector<string> tokens;
    char* begin = strdup(rawString.c_str());
    char* ptr = strtok(begin," ");
    while (ptr != NULL) {
        tokens.push_back(string(ptr));
        ptr = strtok(NULL," ");
    }
    delete begin;
    return tokens;
}
Expression* Calculator::treeBuilder(vector<string> RPNtokens) {
    stack<Expression*> expStack;
    for (string token : RPNtokens) {
        if (isOperation(token)) {
            if (expStack.size() < 2) {
                throw runtime_error("Failed to parse expression. Please check your syntax.\n");
            }
            Expression* rightTerm = expStack.top();
            expStack.pop();
            Expression* leftTerm = expStack.top();
            expStack.pop();
            if (token == "+" || token == "-") {
                if (token == "-")
                    rightTerm->negate();
                expStack.push(new Addition(leftTerm,rightTerm));
            }
            else if (token == "*") {
                expStack.push(new Multiplication(leftTerm,rightTerm));
            }
            else if (token == "/") {
                expStack.push(new Division(leftTerm,rightTerm));
            }
            else if (token == "^") {
                expStack.push(new Exponentiation(false,leftTerm,rightTerm));
            }
            else if (token == "rt") {
                Division* exponent = new Division(new Integer(1),leftTerm);
                expStack.push(new Exponentiation(false,rightTerm,exponent));
            }
            else {
                throw runtime_error("Unsupported operation detected.\n");
            }
        }
        else if (token == "ans") {
            if (prevAnswers.size() == 0) {
                throw runtime_error("No previous answers are available.\n");
            }
            expStack.push(prevAnswers.front()->duplicate());
        }
        else {
            try {
                expStack.push(new Integer(stoi(token)));
            }
            catch (exception& e) {
                stringstream errorString;
                errorString << "Invalid token '" << token <<"'"<<endl;
                throw runtime_error(errorString.str());
            }
        }
    }
    if (expStack.size() != 1) {
        throw runtime_error("Failed to parse expression. Please check your syntax.\n");
    }
    return expStack.top();
}
void Calculator::solve(std::string input) {
    vector<string> tokens = tokenizer(input);
    try {
        vector<string> RPN_tokens = convertToRPN(tokens);
        Expression* root = treeBuilder(RPN_tokens);
        Expression* simplified = root->simplify();
        delete root;
        cout << simplified->toString() << endl;
        prevAnswers.push_front(simplified);
        if (prevAnswers.size() >= 5 ) //store only the 5 recent answers
            prevAnswers.pop_back();
    } catch (exception &e) {
        cerr << "Error:: " << e.what() << endl;
    }
}

Calculator::~Calculator() {
    for (auto expr : prevAnswers)
        delete expr;
}