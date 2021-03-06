//
//  Division.cpp
//  COP3503_Project
//
//  Created by Ben on 7/25/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Division.hpp"
#include "Integer.hpp"
#include "Addition.hpp"
#include "Multiplication.hpp"
#include "Exponentiation.hpp"
#include <stdexcept>
#include <iostream>
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
Expression* Division::factorsToMultExpr(std::vector<Expression*> factors) {
    if (factors.size() == 1)
        return factors.front();
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
void Division::rationalizeFactors(std::vector<Expression*> &numFactors, std::vector<Expression*> &denomFactors) {
    for (int i = 0; i < denomFactors.size(); i++) {
        Exponentiation* factor = dynamic_cast<Exponentiation*>(denomFactors[i]);
        //isRoot() makes sure that expoFactor has an exponent of type division of two Integers
        if (factor != nullptr && factor->isRoot()) {
            auto divExpo = dynamic_cast<Division*>(factor->getRightSide());
            auto numerator = dynamic_cast<Integer*>(divExpo->getLeftSide());
            auto denominator = dynamic_cast<Integer*>(divExpo->getRightSide());
            
            int newNumerator = denominator->getValue() - numerator->getValue() % denominator->getValue();
            Division* newExponent = new Division(new Integer(newNumerator),denominator->duplicate());
            auto rationalizingExpr = new Exponentiation(factor->isNegative(),factor->getLeftSide()->duplicate(),newExponent);
            auto replacedTerm =factor->getLeftSide()->duplicate();
            delete denomFactors[i]; // remove the root factor from the denom factors
            denomFactors[i] = replacedTerm;
            numFactors.push_back(rationalizingExpr);
        }
    }
}
Division::Division(Expression* ls,Expression* rs){
    double denomValue = rs->getDecimalRepresentation();
    if (abs(denomValue) < 5e-10) {
        stringstream errorString;
        errorString << "Zero denominator: " << rs->toString() << endl;
        errorString << "\t\tin the following fraction: ";
        errorString << "(" << ls->toString() << ")/(" << rs->toString() <<")" << endl;
        throw runtime_error(errorString.str());
    }
    leftSide = ls; //numerator
    rightSide = rs; //denominator
}
double Division::getDecimalRepresentation() {
    return leftSide->getDecimalRepresentation() / rightSide->getDecimalRepresentation();
}
std::vector<Expression*> Division::getNumeratorFactors(bool breakIntoPrimes) {
    return leftSide->getNumeratorFactors(breakIntoPrimes);
}
std::vector<Expression*> Division::getDenominatorFactors(bool breakIntoPrimes) {
    return rightSide->getNumeratorFactors(breakIntoPrimes); //get the factors and break them
}
std::vector<Expression*> Division::getAdditiveTerms() {
    vector<Expression*> terms;
    terms.push_back(this->duplicate());
    return terms;
}
Expression* Division::simplify() {
    Expression* simplifiedNumerator = leftSide->simplify();
    Expression* simplifiedDenominator = rightSide->simplify();
    //treat the numerator and the denominator as fractions.
    //all non-fraction types return denominator as Integer(1)
    vector<Expression*> numNumFactors = simplifiedNumerator->getNumeratorFactors(true);
    vector<Expression*> denomDenomFactors = simplifiedDenominator->getDenominatorFactors(true);
    vector<Expression*> combinedNumFactors = combineExpressions(numNumFactors, denomDenomFactors);
    
    vector<Expression*> numDenomFactors = simplifiedNumerator->getDenominatorFactors(true);
    vector<Expression*> denomNumFactors = simplifiedDenominator->getNumeratorFactors(true);
    vector<Expression*> combinedDenomFactors = combineExpressions(numDenomFactors, denomNumFactors);
    delete simplifiedNumerator; //dispose of simplified num and denum since we already got their factors
    delete simplifiedDenominator; // and won't be using them again
    // remove all the common factors
    for (auto& denomFactor : combinedDenomFactors) {
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
    rationalizeFactors(combinedNumFactors,combinedDenomFactors);
    // filter all the nulls and 1's
    Integer numberOne(1),numberZero(0);
    vector<Expression*> filteredNum;
    for (auto factor : combinedNumFactors) {
        if (factor != nullptr && !factor->isEqual(&numberOne))
            filteredNum.push_back(factor);
    }
    vector<Expression*> filteredDenom;
    for (auto factor : combinedDenomFactors) {
        if (factor != nullptr && !factor->isEqual(&numberOne))
            filteredDenom.push_back(factor);
    }
    if (filteredDenom.size() == 0) { //we no longer have a denominator
        Expression* newNum = factorsToMultExpr(filteredNum);
        Expression* simplifiedNum = newNum->simplify();
        delete newNum;
        return simplifiedNum;
    }
    // else, we still have a fraction, so return a new simplified division
    Expression* newNum = factorsToMultExpr(filteredNum);
    Expression* simplifiedNum = newNum->simplify();
    delete newNum;
    Expression* newDenom = factorsToMultExpr(filteredDenom);
    Expression* simplifiedDenom = newDenom->simplify();
    delete newDenom;
    if (simplifiedNum->isEqual(&numberZero))
        return new Integer(0);
    return new Division(simplifiedNum,simplifiedDenom);
}
std::string Division::toString() {
    stringstream str;
    Expression* copyDiv = this->duplicate();
    Expression* copyLeftSide = copyDiv->getLeftSide();
    Expression* copyRighSide = copyDiv->getRightSide();
    if (copyDiv->isNegative()) { //if the overall expression is negative, print a -
        str << "-";
    }
    // don't print negatives
    if (copyLeftSide->isNegative())
        copyLeftSide->negate();
    if (copyRighSide->isNegative())
        copyRighSide->negate();
    
    // don't print parenthesis for integers or constants
    if (copyLeftSide->isCombinedExpression())
        str << "(" << copyLeftSide->toString() << ")";
    else
        str << copyLeftSide->toString();
    str << "/";
    if (copyRighSide->isCombinedExpression())
        str << "(" << copyRighSide->toString() << ")";
    else
       str << copyRighSide->toString();
    delete copyDiv;
    return str.str();
}
Expression* Division::getLeftSide() {
    return leftSide;
}
Expression* Division::getRightSide() {
    return rightSide;
}
Expression* Division::addExpression(Expression* e) {
    vector<Expression*> thisDenomFactors = getDenominatorFactors(false);
    vector<Expression*> thisNumFactors = getNumeratorFactors(false);
    
    vector<Expression*> thatDenomFactors = e->getDenominatorFactors(false);
    vector<Expression*> thatNumFactors = e->getNumeratorFactors(false);
    // duplicate so we can delete the newFract later on.
    Expression* combinedDenom = factorsToMultExpr(combineExpressions(thisDenomFactors, thatDenomFactors))->duplicate();
    Expression* leftNum = new Multiplication(factorsToMultExpr(thisNumFactors),factorsToMultExpr(thatDenomFactors));
    Expression* rightNum = new Multiplication(factorsToMultExpr(thisDenomFactors),factorsToMultExpr(thatNumFactors));
    
    Expression* newFract = new Division(new Addition(leftNum,rightNum),combinedDenom);
    Expression* simplifiedNewFrac = newFract->simplify();
    delete newFract;
    return simplifiedNewFrac;
}
Expression* Division::multiplyExpression(Expression* e) {
    vector<Expression*> thisDenomFactors = getDenominatorFactors(false);
    vector<Expression*> thisNumFactors = getNumeratorFactors(false);
    
    vector<Expression*> thatDenomFactors = e->getDenominatorFactors(false);
    vector<Expression*> thatNumFactors = e->getNumeratorFactors(false);
    
    Expression* combinedNum = factorsToMultExpr(combineExpressions(thisNumFactors, thatNumFactors));
    Expression* combinedDenom = factorsToMultExpr(combineExpressions(thisDenomFactors, thatDenomFactors));
    Expression* combinedDiv = new Division(combinedNum,combinedDenom);
    
    Expression* simplifiedDiv = combinedDiv->simplify();
    delete combinedDiv;
    return simplifiedDiv;
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
bool Division::isEqual(Expression* e) {
    Division* thatFraction = dynamic_cast<Division*>(e);
    if ( thatFraction == nullptr) {
        return false;
    }
    if (this->isNegative() != thatFraction->isNegative())
        return false;
    return leftSide->isEqual(thatFraction->getLeftSide()) && rightSide->isEqual(thatFraction->getRightSide());
}
bool Division::isCombinedExpression() {
    return true;
}
Division::~Division() {
    delete leftSide;
    delete rightSide;
}