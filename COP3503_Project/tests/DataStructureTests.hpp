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
#include "Division.hpp"
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
void MultiplicationIsEqualTest() {
    cout << "Start MultiplicationIsEqualTest: " << endl;
    Expression* s1 = new Integer(-5);
    Expression* s2 = new Integer(-2);
    Expression* s3 = new Addition(s1,s2);
    Expression* s11 = new Integer(-2);
    Expression* s4 = new Multiplication(s11,s3);
    
    Expression* q1 = new Integer(5);
    Expression* q2 = new Integer(2);
    Expression* q3 = new Addition(q1,q2);
    Expression* q11 = new Integer(2);
    Expression* q4 = new Multiplication(q3,q11);
    
    cout << q4->isEqual(s4) << endl;
    cout << s4->isEqual(q4) << endl;
    cout << s4->toString() << " = " << q4->toString() << endl;
    cout << "value: " << q4->getDecimalRepresentation() <<endl;
    cout << "value: " << s4->getDecimalRepresentation() <<endl;
    cout << "End MultiplicationIsEqualTest" << endl;
}
void DivisionToStringTest() {
    cout << "Start DivisionToStringTest: " << endl;
    Expression* s1 = new Integer(5);
    Expression* s2 = new Integer(-2);
    Expression* s3 = new Multiplication(s1,s2);
    
    auto a1 = new Integer(100);
    auto d1 = new Division(s3,a1);
    cout << d1->toString() << endl;
    cout << "End DivisionToStringTest" << endl;
}
void MultiplicationGetFactorsTest() {
    cout << "Start MultiplicationGetFactorsTest: " << endl;
    Expression* s1 = new Integer(5);
    Expression* s2 = new Integer(-2);
    Expression* s3 = new Division(s1,s2);
    
    Expression* s11 = new Integer(7);
    Expression* s12 = new Integer(10);
    Expression* s13 = new Division(s11,s12);
    
    Expression* i11 = new Integer(20);
    Expression* i12 = new Integer(37);
    Expression* i13 = new Multiplication(i11,i12);
    
    auto m1 = new Multiplication(s13,i13);
    
    auto m2 = new Multiplication(m1,s3);
    cout << "m2: " << m2->toString() << endl;
    auto factors = m2->getFactors();
    cout << "End MultiplicationGetFactorsTest" << endl;
}
void isEqual() {
    auto one = new Integer(1);
    Integer oneStack(2);
    if (one->isEqual(&oneStack)) {
        cout << endl << "equal" << endl;
    }
    else
        cout << endl << "not equal" << endl;
}
void typeIdTest() {
    Multiplication* copyLeftSide = new Multiplication(new Integer(1),new Integer(2));
    if ( typeid(copyLeftSide) != typeid(Multiplication *))
        cout <<"unequal types";
    else
        cout <<"equal types";
    cout << endl;
}
#endif /* DataStructureTests_cpp */
