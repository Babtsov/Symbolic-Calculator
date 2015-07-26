//
//  Addition.cpp
//  COP3503_Project
//
//  Created by Ben on 7/21/15.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Addition.hpp"
using namespace std;

Addition::Addition(Expression* ls,Expression* rs) {
    leftSide = ls;
    rightSide = rs;
}
double Addition::getDecimalRepresentation() {
    return leftSide->getDecimalRepresentation() + rightSide->getDecimalRepresentation();
}
std::vector<Expression*> Addition::getNumeratorFactors(bool breakIntoPrimes = false) {
    vector<Expression*> factors;
    factors.push_back(this->duplicate());
    return factors;
}
std::vector<Expression*> Addition::getDenominatorFactors() {
    vector<Expression*> factors;
    factors.push_back(new Integer(1));
    return factors;
}

Expression* Addition::simplify() {
    // the key for simplifying addition expression is simplifying the whole addition chain.
    vector<Expression*> additiveTerms = getAdditiveTerms(); // get all the atoms
    vector<Expression*> simplifiedTerms;
    // make sure we simplify the atoms before trying to add them up
    for (Expression* exp : additiveTerms) {
        simplifiedTerms.push_back(exp->simplify());
    }
    
    for (int i = 0; i < simplifiedTerms.size() - 1; i++) {
        if (simplifiedTerms[i] == nullptr)
            continue;
        for (int j = i + 1; j < simplifiedTerms.size(); j++) {
            if (simplifiedTerms[j] == nullptr)
                continue;
            Expression* sum = simplifiedTerms[i]->addExpression(simplifiedTerms[j]);
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
        itemsToReturn.push(new Addition(item1, item2));
    }
    return itemsToReturn.top();
}
std::vector<Expression*> Addition::getAdditiveTerms() {
    vector<Expression*> additiveTerms;
    for (auto expression : leftSide->getAdditiveTerms()) {
        additiveTerms.push_back(expression);
    }
    for (auto expression : rightSide->getAdditiveTerms()) {
        additiveTerms.push_back(expression);
    }
    return additiveTerms;
}

std::string Addition::toString(){
    stringstream str;
    auto terms = getAdditiveTerms();
    
    assert(terms.size() > 1); //make sure we have at least 2 terms to print
    
    str << terms[0]->toString();
    for (int i = 1; i < terms.size(); i++) {
            if (terms[i]->isNegative())
                str << terms[i]->toString(); // negative representation is taken care of by the term
            else
                str << "+" <<terms[i]->toString();
    }
    for (auto term : terms) {
        delete term;
    }
    return str.str();
}
Expression* Addition::multiplyExpression(Expression* e) {
    return nullptr;
}
Expression* Addition::duplicate() {
    return new Addition(leftSide->duplicate(),rightSide->duplicate());
}
bool Addition::isEqual(Expression* e) {
    bool valueToReturn = true;
    Addition* thatAddition = dynamic_cast<Addition*>(e);
    if (thatAddition == nullptr)
        return false;
    auto thisAdditiveTerms = this->getAdditiveTerms();
    auto thatAdditiveTerms = thatAddition->getAdditiveTerms();

    for (auto thisTerm : thisAdditiveTerms) {
        bool termFound = false;
        for (auto& thatTerm : thatAdditiveTerms) {
            if (thatTerm == nullptr)
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
    if (thisAdditiveTerms.size() != thatAdditiveTerms.size())
        valueToReturn = false;
    //cleanup
    for (auto term : thisAdditiveTerms) {
        delete term;
    }
    for (auto term : thatAdditiveTerms) {
        if (term != nullptr)
            delete term;
    }
    
    return valueToReturn;
}

