//
//  Multiplication.cpp
//  COP3503_Project
//
//  Created by Ben on 7/23/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Multiplication.hpp"
#include "Exponentiation.hpp"
#include "Division.hpp"
#include "Integer.hpp"
using namespace std;

std::pair< Integer*, std::vector<Expression*> > Multiplication::splitCoefAndFactors(Multiplication* lhs) {
    pair< Integer*, std::vector<Expression*> >  coefFactorsPair;
    Integer* Coefficient = nullptr;
    vector<Expression*> multFactors = lhs->getUnsimplifiedFactors();
    //transfer all the integer factor
    for (auto& factor : multFactors) {
        Integer* intFactor = dynamic_cast<Integer*>(factor);
        if (intFactor != nullptr && Coefficient == nullptr) {
            Coefficient = dynamic_cast<Integer*>(intFactor->duplicate());
            delete factor;
            factor = nullptr;
        }
        else if (intFactor != nullptr && Coefficient != nullptr) {
            assert(0); // we should not have gotten more than 1 int factor per MultExpr
            coefFactorsPair.first = nullptr;
            coefFactorsPair.second = multFactors;
            return coefFactorsPair;
        }
    }
    if (Coefficient == nullptr)
        Coefficient = new Integer(1);
    // filter all the nulls and convert all the factors to positive.
    vector<Expression*> readyFactors; //factors ready for comparison.
    for (auto factor: multFactors){
        if (factor == nullptr)
            continue;
        if (factor->isNegative()) {
            factor->negate();
            readyFactors.push_back(factor);
        }
        else
            readyFactors.push_back(factor);
    }
    coefFactorsPair.first = Coefficient;
    coefFactorsPair.second = readyFactors;
    return coefFactorsPair;
}
Expression* Multiplication::factorsToMultExpr(std::vector<Expression*> factors) {
    stack<Expression*> itemsToReturn;
    if (factors.empty()) {
        assert(0);
    }
    for (auto factor : factors) {
        if (factor == nullptr)
            assert(0);
        itemsToReturn.push(factor);
    }
    while (itemsToReturn.size() > 1) {
        Expression* item1 = itemsToReturn.top();
        itemsToReturn.pop();
        Expression* item2 = itemsToReturn.top();
        itemsToReturn.pop();
        itemsToReturn.push(new Multiplication(item2, item1));
    }
    return itemsToReturn.top();
}
std::vector<Expression*> Multiplication::getUnsimplifiedFactors() {
    vector<Expression*> factors;
    Multiplication* lsMult = dynamic_cast<Multiplication*>(leftSide);
    if (lsMult != nullptr) {
        for (auto term: lsMult->getUnsimplifiedFactors())
            factors.push_back(term);
    }
    else
        factors.push_back(leftSide->duplicate());
    Multiplication* rsMult = dynamic_cast<Multiplication*>(rightSide);
    if (rsMult != nullptr) {
        for (auto term: rsMult->getUnsimplifiedFactors())
            factors.push_back(term);
    }
    else
        factors.push_back(rightSide->duplicate());
    return factors;
}

std::vector<Expression*> Multiplication::getFactors() {
    vector<Expression*> factors;
    Multiplication* lsMult = dynamic_cast<Multiplication*>(leftSide);
    if (lsMult != nullptr) {
        for (auto term: lsMult->getFactors())
            factors.push_back(term);
    }
    else {
        Expression* lsSimplified = leftSide->simplify();
        Multiplication* lsSimplifiedMult = dynamic_cast<Multiplication*>(lsSimplified);
        if (lsSimplifiedMult != nullptr) {
            for (auto term: lsSimplifiedMult->getFactors())
                factors.push_back(term);
        }
        else
            factors.push_back(lsSimplified);
    }
    
    Multiplication* rsMult = dynamic_cast<Multiplication*>(rightSide);
    if (rsMult != nullptr) {
        for (auto term: rsMult->getFactors())
            factors.push_back(term);
    }
    else {
        Expression* rsSimplified = rightSide->simplify();
        Multiplication* rsSimplifiedMult = dynamic_cast<Multiplication*>(rsSimplified);
        if (rsSimplifiedMult != nullptr) {
            for (auto term: rsSimplifiedMult->getFactors())
                factors.push_back(term);
        }
        else
            factors.push_back(rsSimplified);
    }
        //factors.push_back(rightSide->duplicate());
    
    return factors;
}
Multiplication::Multiplication(Expression* ls,Expression* rs) {
    leftSide = ls;
    rightSide = rs;
}
double Multiplication::getDecimalRepresentation() {
    return leftSide->getDecimalRepresentation() * rightSide->getDecimalRepresentation();
}
std::vector<Expression*> Multiplication::getNumeratorFactors(bool breakIntoPrimes) {
    vector<Expression*> factors;
    for (auto factor : leftSide->getNumeratorFactors(breakIntoPrimes)) {
        factors.push_back(factor);
    }
    for (auto factor : rightSide->getNumeratorFactors(breakIntoPrimes)) {
        factors.push_back(factor);
    }
    return factors;
}
std::vector<Expression*> Multiplication::getDenominatorFactors(bool breakIntoPrimes) {
    vector<Expression*> factors;
    factors.push_back(new Integer(1));
    return factors;
}
std::vector<Expression*> Multiplication::getAdditiveTerms() {
    vector<Expression*> terms;
    terms.push_back(this->duplicate());
    return terms;
}
Expression* Multiplication::simplify() {
    // the key for simplifying Mult expression is simplifying the whole multiplication chain.
    vector<Expression*> simplifiedTerms = getFactors(); // get all the atoms

    bool combOccured; //a flag to test if a combination occured. if no combination occured at all, we are done multiplying
    do {
        combOccured = false;
        for (int i = 0; i < simplifiedTerms.size() - 1; i++) {
            if (simplifiedTerms[i] == nullptr)
                continue;
            for (int j = i + 1; j < simplifiedTerms.size(); j++) {
                if (simplifiedTerms[j] == nullptr || simplifiedTerms[i] == nullptr)
                    continue;
                Expression* product = simplifiedTerms[i]->multiplyExpression(simplifiedTerms[j]);
                Multiplication* productAsMult = dynamic_cast<Multiplication*>(product);
                if (product != nullptr) {
                    delete simplifiedTerms[i];
                    delete simplifiedTerms[j];
                    simplifiedTerms[i] = nullptr;
                    simplifiedTerms[j] = nullptr;
                    combOccured = true;
                }
                if (product != nullptr && productAsMult == nullptr)
                    simplifiedTerms.push_back(product);
                else if(product != nullptr && productAsMult != nullptr) {
                    simplifiedTerms.push_back(productAsMult->getLeftSide()->duplicate());
                    simplifiedTerms.push_back(productAsMult->getRightSide()->duplicate());
                    delete productAsMult;
                }
            }
        }
    } while(combOccured);
    // clean up the array of simplified terms from null pointers
    stack<Expression*> itemsToReturn;
    for (Expression* exp : simplifiedTerms) {
        if (exp == nullptr) {
            continue;
        }
        itemsToReturn.push(exp);
    }
    // build the new simplified multiplication tree
    while (itemsToReturn.size() > 1) {
        Expression* item1 = itemsToReturn.top();
        itemsToReturn.pop();
        Expression* item2 = itemsToReturn.top();
        itemsToReturn.pop();
        itemsToReturn.push(new Multiplication(item1, item2));
    }
    assert(itemsToReturn.size() == 1);
    return itemsToReturn.top();
}
std::string Multiplication::toString(){
    stringstream str;
    auto terms = getUnsimplifiedFactors();
    
    assert(terms.size() > 1); //make sure we have at least 2 terms to print
    if (this->isNegative()) { //if the overall expression is negative, print a -
        str << "-";
    }
    for (int i = 0; i < terms.size() - 1; i++) {
        if (terms[i]->isNegative()) //make sure we print all terms as positive
            terms[i]->negate();     // since we took care of the overall negative
        
        if (terms[i]->getLeftSide() != nullptr && terms[i]->getRightSide() != nullptr)
            str << "(" << terms[i]->toString() <<")" << "*"; //means term is some kind of compound expression.
        else
            str <<terms[i]->toString() << "*";
    }
    if (terms[terms.size() - 1]->isNegative()) //make sure we print each term
        terms[terms.size() - 1]->negate(); // as a positive since we took care of the overall negative
    if (terms[terms.size() - 1]->getLeftSide() != nullptr && terms[terms.size() - 1]->getRightSide() != nullptr)
        str << "(" << terms[terms.size() - 1]->toString() <<")"; //means term is some kind of compound expression.
    else
        str <<terms[terms.size() - 1]->toString();
    
    for (auto term : terms)
        delete term;

    return str.str();
}
Expression* Multiplication::getLeftSide(){
    return leftSide;
}
Expression* Multiplication::getRightSide() {
    return rightSide;
}

Expression* Multiplication::addExpression(Expression* e){
    Division* divExpr = dynamic_cast<Division*>(e);
    if (divExpr != nullptr) {
        return divExpr->addExpression(this);
    }
    Multiplication* multExpr = dynamic_cast<Multiplication*>(e);
    Exponentiation* expoExpr = dynamic_cast<Exponentiation*>(e);
    if (multExpr != nullptr || expoExpr != nullptr) {
        pair< Integer*, std::vector<Expression*> > thatCoefAndFactors;
        if (expoExpr != nullptr) {
            auto expoToMult = new Multiplication(new Integer(1),expoExpr);
            thatCoefAndFactors = splitCoefAndFactors(expoToMult);
        }
        else
            thatCoefAndFactors = splitCoefAndFactors(multExpr);

        vector<Expression*> thatFactors =thatCoefAndFactors.second;
        Integer* thatCoef = thatCoefAndFactors.first;
        
        auto thisCoefAndFactors = splitCoefAndFactors(this);
        Integer* thisCoef = thisCoefAndFactors.first;
        vector<Expression*> thisFactors =thisCoefAndFactors.second;
        
        
        if (thisCoef == nullptr || thatCoef == nullptr)
            return nullptr;
        if (thisFactors.size() != thatFactors.size())
            return nullptr;
        //perform term by term comparison to make sure we can add the two terms
        for (int i = 0; i < thisFactors.size(); i++) {
            if (!thisFactors[i]->isEqual(thatFactors[i]))
                return nullptr;
        }
        // at this point, we guaretee that all the factors are equal, so add integers
        Integer* coefSum = new Integer(thisCoef->getValue() + thatCoef->getValue());
        delete thisCoef;
        delete thatCoef;
        for (auto factor : thatFactors)
            delete factor;
        
        return new Multiplication(coefSum,factorsToMultExpr(thisFactors));
    }
    return nullptr;
}
Expression* Multiplication::multiplyExpression(Expression* e) {
    assert(0);
    return nullptr;
}
Expression* Multiplication::duplicate(){
    return new Multiplication(leftSide->duplicate(),rightSide->duplicate());
}
void Multiplication::negate(){
    leftSide->negate();
}
bool Multiplication::isNegative() {
    int negativeFactors = 0;
    for (auto term: this->getUnsimplifiedFactors()) {
        if (term -> isNegative())
            negativeFactors++;
        delete term;
    }
    return negativeFactors % 2 != 0;
}
bool Multiplication::isEqual(Expression* e) {
    bool valueToReturn = true;
    Multiplication* thatMultiplication = dynamic_cast<Multiplication*>(e);
    if (thatMultiplication == nullptr)
        return false;
    if (this->isNegative() != thatMultiplication->isNegative())
        return false;
    // since they are the same sign, make both of them positive for a valid comparison.
    if (this->isNegative())
        this->negate();
    if (thatMultiplication->isNegative())
        thatMultiplication->negate();
    
    auto thisFactors = this->getUnsimplifiedFactors();
    auto thatFactors = thatMultiplication->getUnsimplifiedFactors();
    
    for (auto thisTerm : thisFactors) {
        if (thisTerm->isNegative()) //ignore all the negative 1's
            thisTerm->negate();
        bool termFound = false;
        for (auto& thatTerm : thatFactors) {
            if (thatTerm == nullptr)
                continue;
            if (thatTerm->isNegative())
                thatTerm->negate();
            
            if (thisTerm->isEqual(thatTerm)) {
                termFound = true;
                delete thatTerm;
                thatTerm = nullptr;
                break;
            }
        }
        if (!termFound){
            valueToReturn = false;
            break;
        }
    }
    if (thisFactors.size() != thatFactors.size())
        valueToReturn = false;
    //cleanup
    for (auto term : thisFactors)
        delete term;
    for (auto term : thatFactors) {
        if (term != nullptr)
            delete term;
    }
    return valueToReturn;
}
Multiplication::~Multiplication(){
    delete leftSide;
    delete rightSide;
}