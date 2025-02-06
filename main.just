@PRESTACK 1 // Expects one command-line argument (the number to calculate the factorial of)
MAIN // Entry point of the program

// Define variables
DEF result 1 // Holds the result of the factorial calculation
DEF index $ // Holds the input number
DEF temp  // Temporary variable for calculations

// Define a label for the loop
LABEL factorial_loop

GET index
PUSH 0
ISLESSER
JMPZ $ done // If n is 0, jump to the end

// Multiply result by n and store it in result
GET result
GET index
MUL
SET result $

// Decrement n
GET index
DEC
SET index $

// Jump back to the start of the loop
JMP $ factorial_loop

// Label for the end of the program
LABEL done

// Print the result
GET result
OUT

// Terminate the program
DONE
