//
//  DataStructureTests.hpp
//  COP3503_Project
//
//  Created by Ben on 7/24/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#ifndef DataStructureTests_cpp
#define DataStructureTests_cpp

#include "Tokenizer.hpp"
#include "Expression.hpp"
#include "Addition.hpp"
#include "Multiplication.hpp"
using namespace std;

void AdditionIsEqualTest(){
    
    cout << "Start AdditionIsEqualTest: " << endl;
    Expression* s1 = new Integer(5);
    Expression* s2 = new Integer(2);
    Expression* s3 = new Addition(s1,s2);
    Expression* s11 = new Integer(5);
    Expression* s4 = new Addition(s11,s3);
    
    Expression* q1 = new Integer(5);
    Expression* q2 = new Integer(2);
    Expression* q3 = new Addition(q1,q2);
    Expression* q11 = new Integer(2);
    Expression* q4 = new Addition(q3,q11);
    
    cout << q4->isEqual(s4) << endl;
    //cout << s4->isEqual(q4) << endl;
    
    cout << "End AdditionIsEqualTest" << endl;
}

#endif /* DataStructureTests_cpp */
