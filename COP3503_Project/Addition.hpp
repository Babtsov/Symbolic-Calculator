//
//  Addition.hpp
//  COP3503_Project
//
//  Created by Ben on 7/21/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#ifndef Addition_cpp
#define Addition_cpp

#include "Expression.hpp"

class Addition : public Expression {
private:
    Expression* leftSide;
    Expression* rightSide;
    static bool sortTerms(Expression *lhs, Expression *rhs);
    
public:
    Addition(Expression* ls,Expression* rs);
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
    virtual bool isCombinedExpression();
    virtual ~Addition();
};

#endif /* Addition_cpp */
