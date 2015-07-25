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
#include "tests/DataStructureTests.hpp"
using namespace std;

Expression* treeBuilder(vector<string> RPNtokens) {
    stack<Expression*> expStack;
    for (string token : RPNtokens) {
        if ( token == "+" || token == "-") {
            Expression* arg1 = expStack.top();
            expStack.pop();
            Expression* arg2 = expStack.top();
            expStack.pop();
            if ( token == "-")
                arg1->negate();
            expStack.push(new Addition(arg2,arg1));
        }
        else if(token == "*") {
            Expression* arg1 = expStack.top();
            expStack.pop();
            Expression* arg2 = expStack.top();
            expStack.pop();
            expStack.push(new Multiplication(arg2,arg1));
        }
        else {
            expStack.push(new Integer(stoi(token)));
        }
    }
    return expStack.top();
}

int main(int argc, const char * argv[]) {
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
    cout << "Unsimplified: "<< root->toString() << endl;
    cout << "Simplified: " << root->simplify()->toString() << endl;

    delete root;
    return 0;
}