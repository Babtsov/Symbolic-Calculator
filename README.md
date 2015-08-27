# Symbolic-Calculator
This is the final project for COP3503 (programming fundamentals for CS majors II). The purpose of this project was to 
build a "symbolic calculator" that can support rationals and irrational numbers symbolically. It can simplify fractions and expressions with roots. 

##  Some example of supported expressions:
* ( 1 - -2 / 3 ) ^ ( 2 rt 16 ) 
* -54 ^ ( 1 / 3 )
* 3 rt ( -4 + 1 / 2 )
* 1 / ( 2 rt 2 ) + 2 rt 2 / 2
* 3 rt 49 * 3 rt 7

## Implementation and design:
* The strategy for implementing the calculator program was to use polymorphism and recursion
* `Expresion` is the abstract base class other classes are derived from. Objects of types derived from this class represent
the mathematical expression in memory. <br />
  **The derived classes are:**
    * `Integer` for representing integers.
    * `Division` for representing rationals and unsimplified division expressions.
    * `Exponentiation` for expresenting irrational numbers (roots) and unsimplified exponentiation expressions.
    * `Multiplication` for representing "non-primitive" multiplication expression.
    * `Addition` for representing "non-primitive" addition expression.
* Each object of the above types holds 2 pointers of type `Expression*` to other expression objects (the `Integer` class is an exception). That way, after the input is tokenized, an expression tree is constructed for each expression entered by the user.
* The most important member fuction of each derived class is `virtual Expression* simplify()`, which each class uses to simplify its expression. `simplify()` of one class (almost always) calls a `simply()` of another class. That way, the whole expression tree can be simplified recursively.

## TODO List:
While this implementation did meet the specs set by the professor of the class, more work needs to be done to ensure a more elegant and correct implementation to this "symbolic calculator" project:
* Implement the tokenizer using regex so that the user won't need to enter a space between each two consecutive tokens.
* Clean up the design and improve code quality
* Deal with memory leaks issues (perhaps consider using smart pointers)
