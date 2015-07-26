//
//  Multiplication.cpp
//  COP3503_Project
//
//  Created by Ben on 7/23/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Multiplication.hpp"
using namespace std;

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
std::vector<Expression*> Multiplication::getDenominatorFactors() {
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
    // the key for simplifying addition expression is simplifying the whole addition chain.
    vector<Expression*> terms = getNumeratorFactors(true); // get all the atoms
    vector<Expression*> simplifiedTerms;
    // make sure we simplify the atoms before trying to add them up
    for (Expression* exp : terms) {
        simplifiedTerms.push_back(exp->simplify());
    }
    
    for (int i = 0; i < simplifiedTerms.size() - 1; i++) {
        if (simplifiedTerms[i] == nullptr)
            continue;
        for (int j = i + 1; j < simplifiedTerms.size(); j++) {
            if (simplifiedTerms[j] == nullptr)
                continue;
            Expression* product = simplifiedTerms[i]->multiplyExpression(simplifiedTerms[j]);
            if (product != nullptr) {
                delete simplifiedTerms[i];
                delete simplifiedTerms[j];
                simplifiedTerms[i] = product;
                simplifiedTerms[j] = nullptr;
            }
        }
    }
    // clean up the array of simplified terms from null pointers
    stack<Expression*> itemsToReturn;
    for (Expression* exp : simplifiedTerms) {
        if (exp == nullptr) {
            continue;
        }
        itemsToReturn.push(exp);
    }
    // build the new simplified addition tree
    while (itemsToReturn.size() > 1) {
        Expression* item1 = itemsToReturn.top();
        itemsToReturn.pop();
        Expression* item2 = itemsToReturn.top();
        itemsToReturn.pop();
        itemsToReturn.push(new Multiplication(item1, item2));
    }
    return itemsToReturn.top();
}
std::string Multiplication::toString(){
    stringstream str;
    auto terms = getNumeratorFactors(false);
    
    assert(terms.size() > 1); //make sure we have at least 2 terms to print
    if (this->isNegative()) { //if the overall expression is negative, print a -
        str << "-";
    }
    for (int i = 0; i < terms.size() - 1; i++) {
        if (terms[i]->isNegative()) //make sure we print all terms as positive since
            terms[i]->negate();     //
        str << "(" << terms[i]->toString() <<")" << "*";
    }
    if (terms[terms.size() - 1]->isNegative()) //make sure we print each term
        terms[terms.size() - 1]->negate(); // as a positive since we took care of the overall negative
    str << "(" << terms[terms.size() - 1]->toString() << ")";
    
    for (auto term : terms) {
        delete term;
    }
    return str.str();
}
Expression* Multiplication::getLeftSide(){
    return leftSide;
}
Expression* Multiplication::getRightSide() {
    return rightSide;
}
Expression* Multiplication::addExpression(Expression* e){ //TODO::
    vector<Expression*> thisFactors = this->getNumeratorFactors(false);
    vector<Expression*> thatFactors = e->getNumeratorFactors(false);
    if (thisFactors.size() != thatFactors.size()) {
        return nullptr;
    }
//    for (auto thisFactor : thisFactors) {
//        if (typeid(*thisFactor) == typeid())
//    }
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
    for (auto term: this->getNumeratorFactors(false)) {
        if (term -> isNegative()) {
            negativeFactors++;
        }
        delete term;
    }
    return negativeFactors % 2 != 0;
}
bool Multiplication::isEqual(Expression* e) {
    bool valueToReturn = true;
    Multiplication* thatMultiplication = dynamic_cast<Multiplication*>(e);
    if (thatMultiplication == nullptr)
        return false;
    auto thisFactors = this->getNumeratorFactors(true); //make sure it separates all the (-1) factors
    auto thatFactors = thatMultiplication->getNumeratorFactors(true);
    
    for (auto thisTerm : thisFactors) {
        if (thisTerm->isNegative()) { //ignore all the negative 1's
            continue;
        }
        bool termFound = false;
        for (auto& thatTerm : thatFactors) {
            if (thatTerm == nullptr || thatTerm->isNegative())
                continue;
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
    if (this->isNegative() != thatMultiplication->isNegative())
        valueToReturn = false;
    //cleanup
    for (auto term : thisFactors) {
        delete term;
    }
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