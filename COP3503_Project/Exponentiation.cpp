//
//  Exponentiation.cpp
//  COP3503_Project
//
//  Created by Ben on 7/27/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Exponentiation.hpp"
#include "Multiplication.hpp"
#include "Division.hpp"
#include "Integer.hpp"
#include <cmath>
#include <iostream>
using namespace std;

Exponentiation::Exponentiation(Expression* ls,Expression* rs) {
    leftSide = ls;
    rightSide = rs;
    isNeg = false;
}

double Exponentiation::getDecimalRepresentation() {
    if (isNeg)
        return -1*pow(leftSide->getDecimalRepresentation(), rightSide->getDecimalRepresentation());
    return pow(leftSide->getDecimalRepresentation(), rightSide->getDecimalRepresentation());
}

std::vector<Expression*> Exponentiation::getNumeratorFactors(bool breakIntoPrimes) {
    vector<Expression*> factors;    
    factors.push_back(new Exponentiation(leftSide->duplicate(),rightSide->duplicate()));
    return factors;
}
std::vector<Expression*> Exponentiation::getDenominatorFactors(bool breakIntoPrimes) {
    vector<Expression*> factors;
    factors.push_back(new Integer(1));
    return factors;
}
std::vector<Expression*> Exponentiation::getAdditiveTerms() {
    vector<Expression*> terms;
    terms.push_back(this->duplicate());
    return terms;
}
Expression* Exponentiation::raiseMult(Multiplication* base, Expression* exponent) {
    vector<Expression*> factors = base->getFactors();
    stack<Expression*> factorsRaised;
    for (auto factor : factors)
        factorsRaised.push(new Exponentiation(factor,exponent->duplicate()));
    
    //build Multiplication tree
    assert(factorsRaised.size() >= 2);
    while (factorsRaised.size() > 1) {
        Expression* item1 = factorsRaised.top();
        factorsRaised.pop();
        Expression* item2 = factorsRaised.top();
        factorsRaised.pop();
        factorsRaised.push(new Multiplication(item1, item2));
    }
    Expression* simplifiedMult = factorsRaised.top()->simplify();
    delete factorsRaised.top();
    return simplifiedMult;
}
Expression* Exponentiation::raiseFraction(Division* divBase, Expression* exponent) {
    // duplicate base so we can delete it later
    auto numDup = divBase->getLeftSide()->duplicate();
    auto denomDup = divBase->getRightSide()->duplicate();
    
    // create the new expression
    auto numDupRaised = new Exponentiation(numDup,exponent->duplicate());
    auto denomDupRaised = new Exponentiation(denomDup,exponent->duplicate());
    
    Division* constructedDiv = new Division(numDupRaised,denomDupRaised);
    Expression* simplifiedDiv = constructedDiv->simplify();
    delete constructedDiv;
    return simplifiedDiv;
}
Expression* Exponentiation::simplifyRoot(Expression* base,Integer* expoNum,Integer* expoDenom) {
    Integer* intBase = dynamic_cast<Integer*>(base);
    if ( expoNum->getValue() >= expoDenom->getValue()) {
        int factoredExpo = expoNum->getValue() / expoDenom->getValue();
        int remainingExpo = expoNum->getValue() % expoDenom->getValue();
        auto factoredExpoExpr = new Exponentiation(base->duplicate(),new Integer(factoredExpo));
        if ( remainingExpo == 0)
            return factoredExpoExpr;
        auto remainExpoExpr = new Exponentiation(base->duplicate(),new Division(new Integer(remainingExpo),expoDenom->duplicate()));
        auto combinedExpr = new Multiplication(remainExpoExpr,factoredExpoExpr);
        auto combinedExprSimplified = combinedExpr->simplify();
        delete combinedExpr;
        return combinedExprSimplified;
    }
    else if (intBase != nullptr) {
        if (intBase->getValue() == 1 || intBase->getValue() == 0)
            return new Integer(intBase->getValue());
        vector<Expression*> primes = intBase->getNumeratorFactors(true);
        if (primes.size() == 1)
            return this->duplicate();
        stack<Expression*> multStack;
        for (auto term : primes) {
            Expression* unsimiplfied = new Exponentiation(term->duplicate(),rightSide->duplicate());
            multStack.push(unsimiplfied->simplify());
            delete unsimiplfied;
        }
        while (multStack.size() > 1) {
            Expression* item1 = multStack.top();
            multStack.pop();
            Expression* item2 = multStack.top();
            multStack.pop();
            multStack.push(new Multiplication(item1, item2));
        }
        Expression* combined = multStack.top();
        Expression* simplified = combined->simplify();
        delete combined;
        return simplified;
    }
    return this->duplicate();
}
Expression* Exponentiation::simplify() {
    Expression* base = leftSide->simplify();
    Expression* exponent = rightSide->simplify();
    // take care of negative exponents 3^(-2) =1/(3^2), notice that preserving the original simplified base and expo
    if (exponent->isNegative()) {
        Integer* newNum = new Integer(1);
        exponent->negate();
        Exponentiation* newDenom = new Exponentiation(base,exponent);
        Expression* combinedExpr = new Division(newNum,newDenom);
        Expression* combinedExprSimplified = combinedExpr->simplify();
        delete combinedExpr;
        return combinedExprSimplified;
    }
    // takes care of when the base is fraction. (3/7)^2 = (3^2)/(7^2)
    Division* divBase = dynamic_cast<Division*>(base);
    if (divBase != nullptr) {
        Expression* simplifiedDivRaised = raiseFraction(divBase,exponent);
        delete base;
        delete exponent;
        return simplifiedDivRaised;
    }
    // takes care when both the base and exponent are integers. 2^3
    Integer* intBase = dynamic_cast<Integer*>(base);
    Integer* intExpo = dynamic_cast<Integer*>(exponent);
    if (intBase != nullptr && intExpo != nullptr) {
        Integer* simplifiedInt = new Integer(pow(intBase->getValue(),intExpo->getValue()));
        delete base;
        delete exponent;
        return simplifiedInt;
    }

    // takes care of when the base is composed of multiplication expression. (2*pi)^2 = 2^2*pi^2
    Multiplication* multBase = dynamic_cast<Multiplication*>(base);
    if (multBase != nullptr) {
        Expression* simplifiedMultRaised = raiseMult(multBase, exponent);
        delete base;
        delete exponent;
        return simplifiedMultRaised;
    }
    // takes care when the exponent is a fraction 2^(1/3)
    Division* divExpo = dynamic_cast<Division*>(exponent);
    if (divExpo != nullptr) {
        Integer* expoNum =  dynamic_cast<Integer*>(divExpo->getLeftSide());
        Integer* expoDenom =  dynamic_cast<Integer*>(divExpo->getRightSide());
        if (expoNum == nullptr || expoDenom == nullptr) {
            std::cerr << "Error:: the following expression: ";
            std::cerr << this->toString() << endl;
            std::cerr << "has an unsupported exponent: " << rightSide->toString() << endl;
        }
        
        Expression* simplifiedRootExpr = simplifyRoot(leftSide,expoNum,expoDenom);
        delete base;
        delete exponent;
        return simplifiedRootExpr;
    }
    
    Expression* defaultSimplified = new Exponentiation(base,exponent);
    if (this->isNegative())
        defaultSimplified->negate();
    return defaultSimplified;
}
std::string Exponentiation::toString() {
    stringstream str;
    if (isNeg)
        str <<"-";
    if ((leftSide->getLeftSide() != nullptr && leftSide->getRightSide() != nullptr) || leftSide->isNegative())
        str << "(" << leftSide->toString() << ")";
    else
        str << leftSide->toString();
    str << "^";
    if ((rightSide->getLeftSide() != nullptr && rightSide->getRightSide() != nullptr) || rightSide->isNegative())
        str << "(" << rightSide->toString() << ")";
    else
        str << rightSide->toString();
    
    return str.str();
}
Expression* Exponentiation::getLeftSide() {
    return leftSide;
}
Expression* Exponentiation::getRightSide() {
    return rightSide;
}
Expression* Exponentiation::addExpression(Expression* e) {
    // handle cases like sqrt(2) + sqrt(2)
    Exponentiation* expoExpr = dynamic_cast<Exponentiation*>(e);
    if (expoExpr != nullptr) {
        // first, get rid of sign to make a valid comparison (using duplicate objects)
        auto dupThis = this->duplicate();
        if (dupThis->isNegative())
            dupThis->negate();
        auto dupThat = expoExpr->duplicate();
        if (dupThat->isNegative())
            dupThat->negate();
        //next, actually compare the duplicates
        if (!dupThis->isEqual(dupThat))
            return nullptr;
        // at this point, they must be equal.
        delete dupThat; //we don't need DupThat anymore since it's equal to dupThis anyway
        if (this->isNegative() && expoExpr->isNegative()) {
            Integer* negTwo = new Integer(-2);
            return new Multiplication(negTwo,dupThis);
        }
        else if (!this->isNegative() && !expoExpr->isNegative()) {
            Integer* Two = new Integer(2);
            return new Multiplication(Two,dupThis);
        }
        else { //the two exponents annihilate each other.
            delete dupThis;
            return new Integer(0);
        }
    }
    // handle addition  handle cases like sqrt(2) + 0
    Integer* intExpr = dynamic_cast<Integer*>(e);
    if (intExpr != nullptr)
        return intExpr->addExpression(this);
    
    return nullptr;
}
Expression* Exponentiation::multiplyExpression(Expression* e) {
    // if expression e is Exponentiation and the two bases are equal to each other
    // add their exponents (and duplicate the base)
    Exponentiation* expoExpr = dynamic_cast<Exponentiation*>(e);
    if (expoExpr != nullptr && this->leftSide->isEqual(expoExpr->getLeftSide())) {
        Expression* thatExponent = expoExpr->getRightSide();
        Expression* combinedExpr = new Exponentiation(leftSide->duplicate(), rightSide->addExpression(thatExponent));
        if (this->isNegative() != expoExpr->isNegative())
            combinedExpr->negate();
        Expression* simpleCombinedExpr = combinedExpr->simplify();
        delete combinedExpr;
        return simpleCombinedExpr;
    }
    //handle multiplication by 1, -1, and 0
    Integer* intExpr = dynamic_cast<Integer*>(e);
    if (intExpr != nullptr)
        return intExpr->multiplyExpression(this);
    
    return nullptr;
}
Expression* Exponentiation::duplicate() {
    Exponentiation* dupExponential = new Exponentiation(leftSide->duplicate(),rightSide->duplicate());
    if (isNeg)
        dupExponential->negate();
    return dupExponential;
}
void Exponentiation::negate() {
    if (isNeg)
        isNeg = false;
    else
        isNeg = true;
}
bool Exponentiation::isNegative() {
    return isNeg;
}
bool Exponentiation::isEqual(Expression* e) {
    Exponentiation* thatExpo = dynamic_cast<Exponentiation*>(e);
    if (thatExpo == nullptr)
        return false;
    if (thatExpo->isNegative() != this->isNegative())
        return false;
    return leftSide->isEqual(e->getLeftSide()) && rightSide->isEqual(e->getRightSide());
}
Exponentiation::~Exponentiation() {
    delete leftSide;
    delete rightSide;
}