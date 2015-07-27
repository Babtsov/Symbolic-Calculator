//
//  Integer.cpp
//  COP3503_Project
//
//  Created by Ben on 7/24/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Integer.hpp"
#include "Division.hpp"

Integer::Integer(int val){
    value = val;
}
int Integer::getValue(){
    return value;
}
double Integer::getDecimalRepresentation(){
    return static_cast<double>(value);
}
Expression* Integer::getLeftSide() {
    return nullptr;
}
Expression* Integer::getRightSide() {
    return nullptr;
}
std::vector<Expression*> Integer::getNumeratorFactors(bool breakIntoPrimes) {
    std::vector<Expression*> numFactors;
    if (value == 1 || value == 0) {
        numFactors.push_back(new Integer(value));
        return numFactors;
    }
    if (!breakIntoPrimes) { //don't break into primes
        numFactors.push_back(new Integer(value));
        return numFactors;
    }
    if (value < 0) {
        numFactors.push_back(new Integer (-1));
    }
    int val = abs(value);
    int divider = 2;
    while (val > 1) {
        if (val % divider == 0) {
            numFactors.push_back(new Integer(divider));
            val /= divider;
            continue;
        }
        divider++;
    }
    return numFactors;
}
std::vector<Expression*> Integer::getDenominatorFactors(bool breakIntoPrimes) {
    std::vector<Expression*> denumFactors;
    denumFactors.push_back(new Integer(1));
    return denumFactors;
}
std::vector<Expression*> Integer::getAdditiveTerms() {
    std::vector<Expression*> addTerms;
    addTerms.push_back(new Integer(value));
    return addTerms;
}
Expression* Integer::simplify() {
    return new Integer(value);
}
Expression* Integer::addExpression(Expression* e) {
    Integer* intExpression = dynamic_cast<Integer*>(e);
    if ( intExpression != nullptr) {
        return new Integer(this->value + intExpression->getValue());
    }
    Division* divExpr = dynamic_cast<Division*>(e);
    if (divExpr != nullptr)
        return divExpr->addExpression(this);
    return nullptr;
}

Expression* Integer::multiplyExpression(Expression* e) {
    Integer* intExpr = dynamic_cast<Integer*>(e);
    if (intExpr != nullptr)
        return new Integer(this->value * intExpr->getValue());
    Division* intDiv = dynamic_cast<Division*>(e);
    if (intDiv != nullptr)
        return intDiv->multiplyExpression(this);

    return nullptr;
}
Expression* Integer::duplicate() {
    return new Integer(value);
}
std::string Integer::toString() {
    std::stringstream str;
    str << value;
    return str.str();
}
void Integer::negate(){
    value *= -1;
}
bool Integer::isNegative() {
    return value < 0;
}
bool Integer::isEqual(Expression* e){
    Integer* intExpression = dynamic_cast<Integer*>(e);
    if (intExpression == nullptr)
        return false;
    return this->value == intExpression->getValue();
}
Integer::~Integer(){
    value = 0;
}
