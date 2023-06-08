; ler o valor 'a' e chama mod2
mod1: BEGIN
mod2: EXTERN
PUBLIC a
PUBLIC volta

SECTION TEXT
INPUT a
jmp mod2
volta: STOP

SECTION DATA
a: SPACE
