//
//  Integer.hpp
//  COP3503_Project
//
//  Created by Ben on 7/24/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#ifndef Integer_cpp
#define Integer_cpp

#include "Expression.hpp"

class Integer: public Expression {
private:
    int value;
public:
    Integer(int val);
    int getValue();
    virtual double getDecimalRepresentation();
    virtual Expression* getLeftSide();
    virtual Expression* getRightSide();
    virtual std::vector<Expression*> getNumeratorFactors(bool breakIntoPrimes);
    virtual std::vector<Expression*> getDenominatorFactors(bool breakIntoPrimes);
    virtual std::vector<Expression*> getAdditiveTerms();
    virtual Expression* simplify();
    virtual Expression* addExpression(Expression* e);
    virtual Expression* multiplyExpression(Expression* e);
    virtual Expression* duplicate();
    virtual std::string toString();
    virtual void negate();
    virtual bool isNegative();
    virtual bool isEqual(Expression* e);
    virtual ~Integer();
};

#endif /* Integer_cpp */
