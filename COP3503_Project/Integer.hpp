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
    virtual std::vector<Expression*> getNumeratorFactors(bool breakIntoPrimes) {
        std::vector<Expression*> numFactors;
        if (value == 1 || 0) {
            numFactors.push_back(new Integer(value));
            return numFactors;
        }
        if (!breakIntoPrimes) { //don't break into primes
            numFactors.push_back(new Integer(value));
            return numFactors;
        }
        if (value < 0) {
            numFactors.push_back(new Integer (-1));
        }
        int val = abs(value);
        int divider = 2;
        while (val > 1) {
            if (val % divider == 0) {
                numFactors.push_back(new Integer(divider));
                val /= divider;
                continue;
            }
            divider++;
        }
        return numFactors;
    }
    virtual std::vector<Expression*> getDenominatorFactors() {
        std::vector<Expression*> denumFactors;
        denumFactors.push_back(new Integer(1));
        return denumFactors;
    }
    virtual std::vector<Expression*> getAdditiveTerms() {
        std::vector<Expression*> addTerms;
        addTerms.push_back(new Integer(value));
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
    //TODO::
    virtual Expression* multiplyExpression(Expression* e) {
        Integer* intExpression = dynamic_cast<Integer*>(e);
        return new Integer(this->value * intExpression->getValue());
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
    virtual bool isNegative() {
        return value < 0;
    }
    virtual bool isEqual(Expression* e){
        Integer* intExpression = dynamic_cast<Integer*>(e);
        if (intExpression == nullptr)
            return false;
        return this->value == intExpression->getValue();
    }
    virtual ~Integer(){}
};


#endif /* Integer_cpp */
