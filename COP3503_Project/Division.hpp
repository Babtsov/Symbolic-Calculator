//
//  Division.hpp
//  COP3503_Project
//
//  Created by Ben on 7/25/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#ifndef Division_cpp
#define Division_cpp

#include "Expression.hpp"

class Division : public Expression {
private:
    Expression* leftSide; //numerator
    Expression* rightSide; //denominator
    std::vector<Expression*> combineExpressions(std::vector<Expression*> lhs,std::vector<Expression*> rhs);
    Expression* factorsToMultExpr(std::vector<Expression*> factors);
    void rationalizeFactors(std::vector<Expression*> &numFactors, std::vector<Expression*> &denomFactors);
    
public:
    Division(Expression* ls,Expression* rs);
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
    virtual ~Division();
};

#endif /* Division_cpp */
