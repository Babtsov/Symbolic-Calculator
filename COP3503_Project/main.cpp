//
//  main.cpp
//  COP3503_Project
//
//  Created by Ben on 7/5/15.
//  compiled and tested with Clang C++11
//  Apple LLVM version 6.1.0 (clang-602.0.53) (based on LLVM 3.6.0svn)
//  Target: x86_64-apple-darwin14.4.0
//  Thread model: posix
//
#include "Calculator.hpp"
#include "Expression.hpp"
#include <stdexcept>
#include <cstdlib>
#include <deque>
#include <iostream>
//#include "tests/DataStructureTests.hpp"
using namespace std;

int mainMenu() {
    string input;
    int inputAsint = 0;
    cout << "Please select an option:" << endl;
    cout << "1) Compute new expression" << endl;
    cout << "2) Show help" << endl;
    cout << "3) Review previous answers" << endl;
    cout << "4) Quit" << endl;
    do {
        cout << "> ";
        getline(cin,input);
        try {
            inputAsint = stoi(input);
            if ( inputAsint < 1 || inputAsint > 4)
                cout << "Invalid number. Try again." << endl;
        }
        catch (exception& e) {
            cout << "Invalid input. Try again." << endl;
        }
    } while ( inputAsint < 1 || inputAsint > 4 );
    return inputAsint;
}
void computeNewExpression(Calculator& calc) {
    cout << "Compute new Expression. Enter \"back\" to go back to main menu or \"quit\" to exit the program." << endl;
    while(1) {
        cout << "> ";
        string inputStr;
        getline(cin,inputStr);
        if (inputStr == "back")
            break;
        if (inputStr == "quit")
            exit(EXIT_SUCCESS);
        calc.solve(inputStr);
    }
    cout << endl;
}
void showHelp(){
    cout << "HOW TO USE THE CALCULATOR" << endl << endl;
    cout << "Enter a mathematical expression to be simplified. Use spaces between numbers, operators and parenthesis." << endl;
    cout << "\tFor example, you can enter \"( 1 - -2 / 3 ) ^ ( 2 rt 16 )\"." << endl;
    cout << "\tNotice that negative numbers are entered without spaces." << endl;
    cout << "To use previous answers, enter \"ans\" in your expression." << endl;
    cout << "\tFor example \"4 + ( ans / 5 ) ^ -2 + ans * 3 rt -2\"." << endl;
    cout << "To take the Nth root of a number enter \"N rt M\" where N is the index and M is the number." << endl;
    cout << "\tFor example, 3 rt -54 is the 3rd (real) root of -54. Notice that unreal roots are unsupported." << endl;
    cout << "\tRoots are the same as a fractinal exponents. The previous example is equivalent to \"-54 ^ ( 1 / 3 )\"" << endl;
    cout << "The following operators are supported +,-,*,/,^,rt. " << endl;
    cout << endl;
    
}
void reviewPrevAns(Calculator& calc) {
    deque<Expression*> previousAnswers = calc.getPreviousAnswers();
    int numOfAnswers = (int)previousAnswers.size();
    if (numOfAnswers == 0) {
        cout << "No previous answers to show." << endl << endl;
        return;
    }
    cout << "Review previous answers (up to 100 answers). Select an answer to see the decimal representation or enter \"back\" to go back." << endl;
    for (int i = 0; i < numOfAnswers; i++) {
        cout << (i + 1) << ") " << previousAnswers[i]->toString() << endl;
    }
    string input;
    while(1) {
        cout << "> ";
        getline(cin,input);
        if (input == "back")
            break;
        try {
            int i = stoi(input);
            if (i < 1 || i > numOfAnswers) {
                cout << "Invalid number. Try again." << endl;
            }
            else {
                cout << previousAnswers[i-1]->toString() << " = ";
                cout << previousAnswers[i-1]->getDecimalRepresentation() << endl;
            }
        }
        catch (exception& e) {
            cout << "Invalid input. Try again." << endl;
        }
    }
    cout << endl;
}
int main(int argc, const char * argv[]) {
    Calculator calc;
    cout << "SYMBOLIC CALCULATOR" << endl << endl;
    while (1) {
        switch (mainMenu()) {
            case 1:
                computeNewExpression(calc);
                break;
            case 2:
                showHelp();
                break;
            case 3:
                reviewPrevAns(calc);
                break;
            default:
                exit(EXIT_SUCCESS);
        }
    }
    return 0;
}