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
#include <stdexcept>
using namespace std;

Expression* treeBuilder(vector<string> RPNtokens) {
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
                expStack.push(new Exponentiation(leftTerm,rightTerm));
            }
            else if (token == "rt") {
                Division* exponent = new Division(new Integer(1),leftTerm);
                expStack.push(new Exponentiation(rightTerm,exponent));
            }
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
    assert(expStack.size() == 1);
    if (expStack.size() != 1) {
        throw runtime_error("Failed to parse expression. Please check your syntax.\n");
    }
    return expStack.top();
}

int main(int argc, const char * argv[]) {
    do {
        cout << "> ";
        string inputStr;
        getline(cin,inputStr);
        vector<string> tokens = tokenizer(inputStr);
        auto RPN_tokens = convertToRPN(tokens);
        try {
            auto root = treeBuilder(RPN_tokens);
            cout << "Unsimplified: "<< root->toString() << " = " << root->getDecimalRepresentation() << endl;
            Expression* simplified = root->simplify();
            cout << "Simplified: " << simplified->toString() << " = " << simplified->getDecimalRepresentation() << endl;
            delete simplified;
            delete root;
        } catch (exception &e) {
            cerr << "Error:: " << e.what() << endl;
        }
        
    } while (1);
    return 0;
}