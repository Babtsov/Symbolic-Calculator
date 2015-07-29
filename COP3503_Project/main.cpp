//
//  main.cpp
//  COP3503_Project
//
//  Created by Ben on 7/5/15.
//  compiled and tested with Clang
//  Apple LLVM version 6.1.0 (clang-602.0.53) (based on LLVM 3.6.0svn)
//  Target: x86_64-apple-darwin14.4.0
//  Thread model: posix
//

#include "Tokenizer.hpp"
#include "Expression.hpp"
#include "Addition.hpp"
#include "Multiplication.hpp"
#include "Exponentiation.hpp"
#include "Division.hpp"
#include "Integer.hpp"
#include "tests/DataStructureTests.hpp"
using namespace std;

Expression* treeBuilder(vector<string> RPNtokens) {
    stack<Expression*> expStack;
    for (string token : RPNtokens) {
        if (isOperation(token)) {
            if (expStack.size() < 2) {
                cout << "Error:: failed to generate Expression tree" << endl;
                return nullptr;
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
                expStack.push(new Exponentiation(leftTerm,rightTerm));
            }
        }
        else {
            expStack.push(new Integer(stoi(token)));
        }
    }
    assert(expStack.size() == 1);
    return expStack.top();
}

int main(int argc, const char * argv[]) {
//    MultiplicationIsEqualTest();
//    DivisionToStringTest();
//    MultiplicationGetFactorsTest();
    do {
    cout << "> ";
// Collect all the tokens from the user
//    stringstream cin;
//    cin << "4 * -5 * ( -3 - -7 ) - 2 + ( 6 * ( 5 - 8 ) - 9 )";
// 4 * -5 - 9 * ( 2 / 7 + 9 ) / ( 8 + 6 / 13 )
// 4 * -5 - 9 * ( 2 / 7 + 9 ) ^ ( 8 + 6 / 13 )

    string inputStr,token;
    getline(cin,inputStr);
    vector<string> tokens = tokenizer(inputStr);
    
    auto RPN_tokens = convertToRPN(tokens);
    
//    cout << "RPN tokens: ";
//    for (auto RPN_token: RPN_tokens){
//        cout<< RPN_token <<" ";
//    }
    cout << endl;
    auto root = treeBuilder(RPN_tokens);
    if (root == nullptr) {
        exit(EXIT_FAILURE);
    }

    cout << "Unsimplified: "<< root->toString() << " = " << root->getDecimalRepresentation() << endl;
    Expression* simplified = root->simplify();
    cout << "Simplified: " << simplified->toString() << " = " << simplified->getDecimalRepresentation() << endl;
    delete simplified;
    delete root;
    } while (0);
    return 0;
}