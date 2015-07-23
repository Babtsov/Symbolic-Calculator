//
//  main.cpp
//  COP3503_Project
//
//  Created by Ben on 7/5/15.
//

#include "Tokenizer.hpp"
#include "Expression.hpp"
#include "Addition.hpp"
#include <typeinfo>
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
//    cin << "3 + 4 * ( 1 + 3 )";

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
//    cout << endl;
//    Expression* s1 = new Integer(80);
//    Expression* s2 = new Integer(10);
//    Expression* s3 = new Addition(s1,s2);
//    
//    Expression* s4 = new Integer(5);
//    Expression* s5 = new Addition(s4,s3);
    
    
//    Expression* simplified = s5->simplify();
//    cout << simplified->toString() << endl;
//    cout << s5->toString() << endl;
    return 0;
}