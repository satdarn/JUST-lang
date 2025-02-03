# JUST uses simple Tools

## A stack based approach to interger computation and manipulation

### In JUST there are 5 types of commands, 

- ### Stack Manipulation
- ### Output
- ### Arithmetic
- ### Variable
- ### Looping

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
There is a "Variable Space" of 26 integers each assigned to the corresponding alphabetic character A-Z
using the $ as the opperand instead of a variable name will pop the value from the stack and use that as opperand

```
SET A x // sets the variable A to the value x
GET A // gets the variable A and pushes to the stack
INCV A // increments the variable A
DECV A // decrements the variable A
```

## Looping

```
LABEL label // sets the jump point for looping
JMP A label // if the variable A isnt 0 jump to label
JMPZ A label // if the variable A is 0 jump to label
```