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

class Expression {
public:
    virtual double getDecimalRepresentation() = 0;
    virtual Expression* getLeftSide() = 0;
    virtual Expression* getRightSide() = 0;
    virtual std::vector<Expression*> getNumeratorFactors() = 0;
    virtual std::vector<Expression*> getDenominatorFactors() = 0;
    virtual std::vector<Expression*> getAdditiveTerms() = 0; //DOESN'T allocate on the heap!
    virtual Expression* simplify() = 0; // allocates on the heap
    virtual Expression* addExpression(Expression* e) = 0;
    virtual Expression* duplicate() = 0;
    virtual void negate() = 0;
    virtual std::string toString() = 0;
    virtual ~Expression(){}
};

class Integer: public Expression{
private:
    int value;
public:
    Integer(int val){
        value = val;
    }
    int getValue(){
        return value;
    }
    virtual double getDecimalRepresentation(){
        return static_cast<double>(value);
    }
    virtual Expression* getLeftSide() {
        return nullptr;
    }
    virtual Expression* getRightSide() {
        return nullptr;
    }
    virtual std::vector<Expression*> getNumeratorFactors() {
        std::vector<Expression*> numFactors;
        numFactors.push_back(new Integer(value));
        return numFactors;
    }
    virtual std::vector<Expression*> getDenominatorFactors() {
        std::vector<Expression*> denumFactors;
        denumFactors.push_back(new Integer(1));
        return denumFactors;
    }
    virtual std::vector<Expression*> getAdditiveTerms() {
        std::vector<Expression*> addTerms;
        addTerms.push_back(this);
        return addTerms;
    }
    virtual Expression* simplify() {
        return new Integer(value);
    }
    virtual Expression* addExpression(Expression* e) {
        Integer* intExpression = dynamic_cast<Integer*>(e);
        if ( intExpression != nullptr) {
            return new Integer(this->value + intExpression->getValue());
        }
        return nullptr;
    }
    virtual Expression* duplicate() {
        return new Integer(value);
    }
    virtual std::string toString() {
        std::stringstream str;
        str << value;
        return str.str();
    }
    virtual void negate(){
        value *= -1;
    }
    virtual ~Integer(){}
};


#endif /* Expression_cpp */
