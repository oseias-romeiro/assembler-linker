; ler o valor 'a' e chama mod2
mod1: BEGIN
EXTERN: mod2
PUBLIC: a

SECTION TEXT
INPUT a
LOAD a
jmp mod2
STOP

SECTION DATA
a: SPACE
