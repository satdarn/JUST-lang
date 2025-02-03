SET A 0    // First Fibonacci number (prev)
SET B 1    // Second Fibonacci number (current)
SET C 10   // Counter for 10 iterations

LABEL loop
GET A      // Push prev
OUT        // Print prev number

GET B      // Push current
GET A      // Push prev
ADD        // Calculate next Fibonacci number

SET A $    // Move current to prev
SET B $    // Store new number as current

DECV C     // Decrease counter
JMP C loop // Repeat if counter != 0

