; conta de 1 a x e mostra a sequencia na tela

SECTION TEXT
INPUT x
LOAD zero

loop: ADD one
STORE res
OUTPUT res
SUB x
JMPZ fim
LOAD res
JMP loop

fim: STOP

SECTION DATA
one: CONST 1
zero: CONST 0
x: SPACE
res: SPACE
