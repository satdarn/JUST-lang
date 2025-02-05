# JUST uses simple Tools

## A stack based approach to interger computation and manipulation

##Usage:

```PlainText
gcc interpreter.c -o interpreter // gcc or any other c compiler
./interpreter main.just // runs the  JUST program

// Preloading stack with values from the command line
./interpreter main.just 10 20 400
```

## An example of a valid program
```PlainText
@PRESTACK 0 // not command line arguments required
MAIN // entry point of the program
DEF var1 10 // defines var1 in the var space and assigns it to the value 10
GET var1 // pushes the value of var1 to the stack
INCV var1 // increments the value of var1
OUT // pops and prints the value from the top of the stack
DONE
```



## Stack Manipulation

```PlainText
PUSH x // pushes the integer value x to the stack
POP // removes the integer value from the stack
SWAP // swaps the last two values of the stack
DUP // duplicates the last value in the stack
```

a note on stack preloading: in order to do so with out proper error handling this syntax should be the first line in the program


```PlainText
@PRESTACK x //where x is the number of values that should be preloaded into the stack
```


## Output

```PlainText
OUT // outputs the last value of the stack
PRINT x // prints the variable x's value ($ syntx applies to this opperand)  
OUTC // outputs the ascii character associated with the last value of the stack
PRINTC x // prints the variable x's value as a ascii character 
```
note: $ syntax can be used with print and printc as they work with variables


## Arithmetic
``` PlainText
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

```PlainText
DEF var x // creates a variable with name "var" and initalizes it with the value x
SET var x // sets the variable "var" to the value x
GET var // gets the variable "var" and pushes to the stack
INCV var // increments the variable "var"
DECV var // decrements the variable "var"
VARINDEX var // pushes the variable "var" 's position in the var space to the stack
```

## Looping

The $ operator also applies to the variable for the conditionals for looping
The $ operator also applies to the label as the value in the stack will be used to pull a label from the index label space
```PlainText
LABEL label // sets the jump point for looping
JMP var label // if the variable var isnt 0 jump to label
JMPZ var label // if the variable var is 0 jump to label
JMPINDEX label // gets the index of the label and push the value to the stack
```


## Macros

These are a way to have repeted chunck of code condensed into a single call,
they have to be defined before the main entry point and terminate them properly

```PlainText
MACRO name // defines the macro with name 
END // Ends the macro and returns to where it was called
CALL name // calls the macro
```

Example:
```PlainText
MACRO check_a_odd
PUSH 2
GET A
MOD
END

MAIN
DEF A 10
CALL check_a_odd
OUT
INCV A
CALL check_a_odd
OUT 
DONE
```