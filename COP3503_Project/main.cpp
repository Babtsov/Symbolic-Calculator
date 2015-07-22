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



int main(int argc, const char * argv[]) {
//    cout << "> ";
    //Collect all the tokens from the user
//    stringstream cin;
//    cin << "3 + 4 * ( 1 + 3 )";

//    string inputStr,token;
//    getline(cin,inputStr);
//    vector<string> tokens = tokenizer(inputStr);
//    
//    auto RPN_tokens = convertToRPN(tokens);
//    
//    
//    for (auto RPN_token: RPN_tokens){
//        cout<< RPN_token <<" ";
//    }
//    cout << endl;
    Expression* s1 = new Integer(80);
    Expression* s2 = new Integer(10);
    Expression* s3 = new Addition(s1,s2);
    
    Expression* s4 = new Integer(5);
    Expression* s5 = new Addition(s4,s3);
    
    
    Expression* simplified = s5->simplify();
    cout << simplified->toString() << endl;
    cout << s5->toString() << endl;
    return 0;
}