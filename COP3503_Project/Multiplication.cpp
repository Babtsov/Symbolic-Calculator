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
    for (auto factor : leftSide->getNumeratorFactors(false)) {
        factors.push_back(factor);
    }
    for (auto factor : rightSide->getNumeratorFactors(false)) {
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
            Expression* sum = simplifiedTerms[i]->multiplyExpression(simplifiedTerms[j]);
            if (sum != nullptr) {
                delete simplifiedTerms[i];
                delete simplifiedTerms[j];
                simplifiedTerms[i] = sum;
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
    if (this->isNegative()) {
        str << "-";
    }
    for (int i = 0; i < terms.size() - 1; i++) {
        if (terms[i]->isNegative()) //make sure we print all terms as positive since
            terms[i]->negate();     //
        str << "(" << terms[i]->toString() <<")" << "*";
    }
    if (terms[terms.size() - 1]->isNegative())
        terms[terms.size() - 1]->negate();
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
    Multiplication* duplicate = new Multiplication(leftSide->duplicate(),rightSide->duplicate());
    return duplicate;
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
    return false;
}
Multiplication::~Multiplication(){
    delete Multiplication::leftSide;
    delete Multiplication::rightSide;
}