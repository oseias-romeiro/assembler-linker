MOD_B:  BEGIN
VAL:    EXTERN
L1:     EXTERN
PUBLIC  Y
PUBLIC  MOD_B

SECTION TEXT
OUTPUT  Y
OUTPUT  VAL
OUTPUT  Y + 2
JMP     L1

SECTION DATA
Y:      SPACE   3
END
