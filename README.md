# JUST uses simple Tools

## A stack based approach to interger computation and manipulation

### In JUST there are 5 types of commands... (kinda)

- ### Stack Manipulation
- ### Output
- ### Arithmetic
- ### Variable
- ### Looping

there is also "macros/subroutines"... but those are yet to be implemented yet

Usage:

```
gcc interpreter.c -o interpreter // gcc or any other c compiler
./interpreter main.just // runs the  JUST program

// Preloading stack with values from the command line
./interpreter main.just 10 20 400
```

## Stack Manipulation

```
PUSH x // pushes the integer value x to the stack
POP // removes the integer value from the stack
SWAP // swaps the last two values of the stack
DUP // duplicates the last value in the stack
```

## Output

```
OUT // outputs the last value of the stack
PRINT x // prints the variable x's value ($ syntx applies to this opperand)
OUTC // outputs the ascii character associated with the last value of the stack
PRINTC x // prints the variable x's value as a ascii character
```


## Arithmetic
``` 
ADD // add the two top elements and pushs the sum
SUB // subtracts the two top elements and pushs the difference
MUL // multiplies the two top elements and pushs the product
DIV // divides the two top elements and pushs the quotient
MOD // divides the two top elements and pushs the remander
INC // increments the top
DEC // decrements the top
```

## Variables

There is a "Variable Space" of size MAX_VARIABLE integers

Using the $ as the opperand will pop the value from the stack and use it as the opperand

In the case of a variable name being replaced with the $, the opperand poped of the stack will
be used to find the variable at the postion in the variable space

```
DEF var x // creates a variable with name "var" and initalizes it with the value x
SET var x // sets the variable "var" to the value x
GET var // gets the variable "var" and pushes to the stack
INCV var // increments the variable "var"
DECV var // decrements the variable "var"
INDEX_OF var // pushes the variable "var" 's position in the var space to the stack
```

## Looping

The $ operator also applies to the variable for the conditionals for looping

```
LABEL label // sets the jump point for looping
JMP var label // if the variable var isnt 0 jump to label
JMPZ var label // if the variable var is 0 jump to label
```