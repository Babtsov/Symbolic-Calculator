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
    Integer* addIntegers(Integer* lhs, Integer* rhs);
    
public:
    Addition(Expression* ls,Expression* rs);
    virtual double getDecimalRepresentation();
    virtual std::vector<Expression*> getNumeratorFactors() {return std::vector<Expression*>();};
    virtual std::vector<Expression*> getDenominatorFactors(){return std::vector<Expression*>();};
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
        assert(1==0);
        return nullptr;
    }
    virtual Expression* duplicate() {
        //return new Addition(leftSide->duplicate(),rightSide->duplicate());
        return nullptr;
    }
    
    virtual ~Addition(){
        delete leftSide;
        delete rightSide;
    }
};

#endif /* Addition_cpp */
