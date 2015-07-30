//
//  Multiplication.hpp
//  COP3503_Project
//
//  Created by Ben on 7/23/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#ifndef Multiplication_cpp
#define Multiplication_cpp

#include "Expression.hpp"
#include "Integer.hpp"
#include <utility>

class Multiplication : public Expression {
private:
    Expression* leftSide;
    Expression* rightSide;
    std::pair< Integer*, std::vector<Expression*> > splitCoefAndFactors(Multiplication* mult);
    Expression* factorsToMultExpr(std::vector<Expression*> factors);
    std::vector<Expression*> getUnsimplifiedFactors(); 
    
public:
    std::vector<Expression*> getFactors();
    Multiplication(Expression* ls,Expression* rs);
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
    virtual ~Multiplication();
};
#endif /* Multiplication_cpp */
