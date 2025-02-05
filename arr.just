@PRESTACK 0
MAIN
DEF ARR0 10 // Create a sudo array as 5 variables in sequence in the var space
DEF ARR1 20
DEF ARR2 30
DEF ARR3 40
DEF ARR4 50
MAIN
VARINDEX ARR0 // Get the index of the first element
DEF X $ // make x the first index
VARINDEX ARR4 // Get the index of the last element
DEF Y $ // make y the last index

LABEL loop
GET X // Push the address of the first element to the stack
PRINT $
INCV X 
DECV Y
JMP Y loop
