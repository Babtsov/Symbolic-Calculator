//
//  main.cpp
//  COP3503_Project
//
//  Created by Ben on 7/5/15.
//

#include "Tokenizer.hpp"
#include "Expression.hpp"
#include "Addition.hpp"
#include "Multiplication.hpp"
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
            Expression* arg1 = expStack.top();
            expStack.pop();
            Expression* arg2 = expStack.top();
            expStack.pop();
            if (token == "+" || token == "-") {
                if (token == "-")
                    arg1->negate();
                expStack.push(new Addition(arg2,arg1));
            }
            else if (token == "*") {
                expStack.push(new Multiplication(arg2,arg1));
            }
            else if (token == "/") {
                Integer* denominatorInt = dynamic_cast<Integer*>(arg1);
                if (denominatorInt != nullptr && denominatorInt->getValue() == 0) {
                    cout << "Error:: Division by zero." <<endl;
                    return nullptr;
                }
                expStack.push(new Division(arg2,arg1));
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
    cout << "> ";
    //Collect all the tokens from the user
//    stringstream cin;
//    cin << "4 * -5 * ( -3 - -7 ) - 2 + ( 6 * ( 5 - 8 ) - 9 )";

    string inputStr,token;
    getline(cin,inputStr);
    vector<string> tokens = tokenizer(inputStr);
    
    auto RPN_tokens = convertToRPN(tokens);
    
    cout << "RPN tokens: ";
    for (auto RPN_token: RPN_tokens){
        cout<< RPN_token <<" ";
    }
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
    return 0;
}