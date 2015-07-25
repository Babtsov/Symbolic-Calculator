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
#include "Integer.hpp"

class Addition : public Expression {
private:
    Expression* leftSide;
    Expression* rightSide;
    
public:
    Addition(Expression* ls,Expression* rs);
    virtual double getDecimalRepresentation();
    virtual std::vector<Expression*> getNumeratorFactors(bool breakIntoPrimes);
    virtual std::vector<Expression*> getDenominatorFactors();
    virtual std::vector<Expression*> getAdditiveTerms();
    virtual Expression* simplify();
    virtual std::string toString();
    
    virtual Expression* getLeftSide() {
        return leftSide;
    }
    virtual Expression* getRightSide() {
        return rightSide;
    }
    virtual Expression* addExpression(Expression* e){
        assert(0); //should not be invoked. denotes an internal error in the system
        return nullptr;
    }
    virtual Expression* multiplyExpression(Expression* e);
    virtual Expression* duplicate();
    virtual void negate(){
        leftSide->negate();
        rightSide->negate();
    }
    virtual bool isNegative() {
        return false;
    }
    virtual bool isEqual(Expression* e);
    virtual ~Addition(){
        delete leftSide;
        delete rightSide;
    }
};

#endif /* Addition_cpp */
