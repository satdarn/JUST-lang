@PRESTACK 0
MAIN
PUSH 5
DEF A $      // Store n in variable A
PUSH 1       // Initialize result as 1
DEF B       // Store result in variable B
SET B $
LABEL loop
GET A        // Push A (n) to stack
JMPZ A end   // If A == 0, jump to end

GET B        // Push B (result) to stack
GET A        // Push A (n) to stack
MUL          // Multiply result by n
SET B $      // Store the new result in B

DECV A       // Decrement n
JMP A loop   // Jump to loop if A != 0

LABEL end
GET B        // Push final result to stack
OUT          // Output the result
