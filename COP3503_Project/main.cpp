//
//  main.cpp
//  COP3503_Project
//
//  Created by Ben on 7/5/15.
//  compiled and tested with Clang
//  Apple LLVM version 6.1.0 (clang-602.0.53) (based on LLVM 3.6.0svn)
//  Target: x86_64-apple-darwin14.4.0
//  Thread model: posix
//
#include "Calculator.hpp"
#include "tests/DataStructureTests.hpp"
using namespace std;

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
}
int main(int argc, const char * argv[]) {
    Calculator calc;
    showHelp();
    do {
        cout << "> ";
        string inputStr;
        getline(cin,inputStr);
        if (inputStr == "back") {
            break;
        }
        calc.solve(inputStr);
    } while (1);
    return 0;
}