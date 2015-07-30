//
//  Exponentiation.hpp
//  COP3503_Project
//
//  Created by Ben on 7/27/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#ifndef Exponentiation_cpp
#define Exponentiation_cpp

#include "Expression.hpp"
#include "Division.hpp"
#include "Integer.hpp"
#include "Multiplication.hpp"

class Exponentiation : public Expression {
private:
    Expression* leftSide; //base
    Expression* rightSide; // exponent
    bool isNeg; //sign to signal if the expression is negative
    Expression* raiseMult(Multiplication* base, Expression* exponent);
    Expression* raiseFraction(Division* divBase, Expression* exponent);
    Expression* simplifyRoot(Expression* base,Integer* expoNum,Integer* expoDenom);
    
public:
    Exponentiation(Expression* ls,Expression* rs);
    virtual double getDecimalRepresentation();
    virtual std::vector<Expression*> getNumeratorFactors(bool breakIntoPrimes);
    virtual std::vector<Expression*> getDenominatorFactors(bool breakIntoPrimes);
    virtual std::vector<Expression*> getAdditiveTerms();
    virtual Expression* simplify();
    virtual std::string toString();
    virtual Expression* getLeftSide();
    virtual Expression* getRightSide();
    virtual Expression* addExpression(Expression* e);
    virtual Expression* multiplyExpression(Expression* e);
    virtual Expression* duplicate();
    virtual void negate();
    virtual bool isNegative();
    virtual bool isEqual(Expression* e);
    virtual ~Exponentiation();
};
#endif /* Exponentiation_cpp */
