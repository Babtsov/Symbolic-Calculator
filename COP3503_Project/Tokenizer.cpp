//
//  Tokenizer.cpp
//  COP3503_Project
//
//  Created by Ben on 7/20/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Tokenizer.hpp"
using namespace std;

// checks if the string is an operation.
bool isOperation(string operation){
    set<string> ops ={"+", "-", "*","/","rt","^"};
    return ops.count(operation) > 0;
}
bool isParenthesis(string token) {
    return (token == "(" || token == ")");
}
// compares the precedence with respect to the lhs.
int comparePrecedence(string lhs,string rhs) {
    map<string,int> precedence = {{"+",0},{"-",0},{"*",1},{"/",1},{"rt",2},{"^",2}};
    return precedence[lhs] - precedence[rhs];
}

// converts the token string to reverse polish notation by using the shunting yard algorithm.
// http://en.wikipedia.org/wiki/Shunting-yard_algorithm
vector<string> convertToRPN(vector<string> tokens) {
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
                cerr << "mismatch in parenthesis 1"<<endl;
                exit(EXIT_FAILURE);
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
            cerr << "mismatch in parenthesis 2"<<endl;
            exit(EXIT_FAILURE);
        }
        reversePolish.push_back(opStack.top());
        opStack.pop();
    }
    return reversePolish;
}
vector<string> tokenizer(const string& rawString) {
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