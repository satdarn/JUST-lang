@PRESTACK 0
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
