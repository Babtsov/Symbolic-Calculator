//
//  Expression.hpp
//  COP3503_Project
//
//  Created by Ben on 7/20/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#ifndef Expression_cpp
#define Expression_cpp

#include <vector>
#include <typeinfo>
#include <string>
#include <stack>
#include <cassert>
#include <sstream>
#include <cmath>

class Expression {
public:
    virtual double getDecimalRepresentation() = 0;
    virtual Expression* getLeftSide() = 0;
    virtual Expression* getRightSide() = 0;
    virtual std::vector<Expression*> getNumeratorFactors(bool breakIntoPrimes) = 0;
    virtual std::vector<Expression*> getDenominatorFactors(bool breakIntoPrimes) = 0;
    virtual std::vector<Expression*> getAdditiveTerms() = 0;
    virtual Expression* simplify() = 0;
    virtual Expression* addExpression(Expression* e) = 0;
    virtual Expression* multiplyExpression(Expression* e) = 0;
    virtual Expression* duplicate() = 0;
    virtual void negate() = 0;
    virtual bool isNegative() = 0;
    virtual bool isEqual(Expression* e) = 0;
    virtual std::string toString() = 0;
    virtual ~Expression() {};
};

#endif /* Expression_cpp */
