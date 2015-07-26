//
//  Division.cpp
//  COP3503_Project
//
//  Created by Ben on 7/25/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Division.hpp"
#include "Integer.hpp"
#include "Multiplication.hpp"
using namespace std;

vector<Expression*> Division::combineExpressions(vector<Expression*> lhs,vector<Expression*> rhs) {
    vector<Expression*> combined;
    for (auto factor : lhs) {
        assert(factor != nullptr);
        combined.push_back(factor);
    }
    for (auto factor : rhs) {
        assert(factor != nullptr);
        combined.push_back(factor);
    }
    return combined;
}
Expression* Division::factorsToMultExpression(std::vector<Expression*> factors) {
    stack<Expression*> itemsToReturn;
    if (factors.empty()) {
        itemsToReturn.push(new Integer(1));
    }
    for (auto factor : factors) {
        if (factor == nullptr)
            assert(0);
        itemsToReturn.push(factor);
    }
    while (itemsToReturn.size() > 1) {
        Expression* item1 = itemsToReturn.top();
        itemsToReturn.pop();
        Expression* item2 = itemsToReturn.top();
        itemsToReturn.pop();
        itemsToReturn.push(new Multiplication(item1, item2));
    }
    return itemsToReturn.top();
}

Division::Division(Expression* ls,Expression* rs){
    leftSide = ls; //numerator
    rightSide = rs; //denominator
}
double Division::getDecimalRepresentation() {
    return leftSide->getDecimalRepresentation() / rightSide->getDecimalRepresentation();
}
std::vector<Expression*> Division::getNumeratorFactors(bool breakIntoPrimes) {
    return leftSide->getNumeratorFactors(breakIntoPrimes);
}
std::vector<Expression*> Division::getDenominatorFactors() {
    return rightSide->getNumeratorFactors(true); //get the factors and break them
}
std::vector<Expression*> Division::getAdditiveTerms() {
    vector<Expression*> terms;
    terms.push_back(this->duplicate());
    return terms;
}
// TODO:: check for division by zero
Expression* Division::simplify() {
    Expression* simplifiedNumerator = leftSide->simplify();
    Expression* simplifiedDenominator = rightSide->simplify();
    //treat the numerator and the denominator as fractions.
    //all non-fraction types return denominator as Integer(1)
    vector<Expression*> numNumFactors = simplifiedNumerator->getNumeratorFactors(true);
    vector<Expression*> denomDenomFactors = simplifiedDenominator->getDenominatorFactors();
    vector<Expression*> combinedNumFactors = combineExpressions(numNumFactors, denomDenomFactors);
    
    vector<Expression*> numDenomFactors = simplifiedNumerator->getDenominatorFactors();
    vector<Expression*> denomNumFactors = simplifiedDenominator->getNumeratorFactors(true);
    vector<Expression*> combinedDenomFactors = combineExpressions(numDenomFactors, denomNumFactors);
    delete simplifiedNumerator; //dispose of simplified num and denum since we already got their factors
    delete simplifiedDenominator; // and won't be using them again
    // remove all the common factors
    for (auto& denomFactor : combinedDenomFactors) {
//        if (denomFactor == nullptr)
//            continue;
        for (auto& numFactor : combinedNumFactors) {
            if (numFactor == nullptr)
                continue;
            if (numFactor->isEqual(denomFactor)) {
                delete numFactor;
                delete denomFactor;
                numFactor = nullptr;
                denomFactor = nullptr;
            }
        }
    }
    // filter all the nulls and 1's
    Integer* numberOne = new Integer(1);
    vector<Expression*> filteredNum;
    for (auto factor : combinedNumFactors) {
        if (factor != nullptr && !factor->isEqual(numberOne))
            filteredNum.push_back(factor);
    }
    vector<Expression*> filteredDenom;
    for (auto factor : combinedDenomFactors) {
        if (factor != nullptr && !factor->isEqual(numberOne))
            filteredDenom.push_back(factor);
    }
    delete numberOne;
    if (filteredDenom.size() == 0) { //we no longer have a denominator
        Expression* newNum = factorsToMultExpression(filteredNum);
        Expression* simplifiedNum = newNum->simplify();
        delete newNum;
        return simplifiedNum;
    }
    Expression* newNum = factorsToMultExpression(filteredNum);
    Expression* simplifiedNum = newNum->simplify();
    delete newNum;
    Expression* newDenom = factorsToMultExpression(filteredDenom);
    Expression* simplifiedDenom = newDenom->simplify();
    delete newDenom;
    return new Division(simplifiedNum,simplifiedDenom);
}
std::string Division::toString() {
    stringstream str;

    if (this->isNegative()) { //if the overall expression is negative, print a -
        str << "-";
    }
    string leftParenthesis,rightParenthesis;
    vector<Expression*> numeratorAdditiveTerms = leftSide->getAdditiveTerms();
    if (numeratorAdditiveTerms.size() == 1) {
        leftParenthesis = "";
        rightParenthesis = "";
    }
    else {
        leftParenthesis = "(";
        rightParenthesis = ")";
    }
    if (leftSide->isNegative())
        leftSide->negate();
    str << leftParenthesis << leftSide->toString() << rightParenthesis;
    for (auto term : numeratorAdditiveTerms) {
        delete term;
    }
    str << "/";
    vector<Expression*> denominatorAdditiveTerms = rightSide->getAdditiveTerms();
    if (denominatorAdditiveTerms.size() == 1) {
        leftParenthesis = "";
        rightParenthesis = "";
    }
    else {
        leftParenthesis = "(";
        rightParenthesis = ")";
    }
    if (rightSide->isNegative())
        rightSide->negate();
    str << leftParenthesis << rightSide->toString() << rightParenthesis;
    for (auto term : denominatorAdditiveTerms) {
        delete term;
    }
    return str.str();
}
Expression* Division::getLeftSide() {
    return leftSide;
}
Expression* Division::getRightSide() {
    return rightSide;
}
Expression* Division::addExpression(Expression* e) {
    Integer* intExp = dynamic_cast<Integer*>(e);
    if (intExp != nullptr) {
        Expression* newNum = rightSide->multiplyExpression(intExp)->addExpression(leftSide);
        Expression* newDenum = rightSide->duplicate();
        Division* unsimplifiedFract = new Division(newNum,newDenum);
        Expression* simplifiedFract = unsimplifiedFract->simplify();
        delete unsimplifiedFract;
        return simplifiedFract;
    }
    return nullptr;
}
Expression* Division::multiplyExpression(Expression* e) {
    return nullptr;
}
Expression* Division::duplicate() {
    return new Division(leftSide->duplicate(),rightSide->duplicate());
}
void Division::negate() {
    leftSide->negate(); //negate just the numerator to negate the whole fraction
}
bool Division::isNegative() {
    return (leftSide->isNegative() != rightSide->isNegative());
}
bool Division::isEqual(Expression* e) { //TODO::
    Division* thatFraction = dynamic_cast<Division*>(e);
    if ( thatFraction == nullptr) {
        return false;
    }
    if (this->isNegative() != thatFraction->isNegative())
        return false;
    return leftSide->isEqual(thatFraction->getLeftSide()) && rightSide->isEqual(thatFraction->getRightSide());
}
Division::~Division() {
    delete leftSide;
    delete rightSide;
}