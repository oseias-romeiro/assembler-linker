; ler o valor 'a' e chama mod_b
mod_a: BEGIN
mod_b: EXTERN
PUBLIC a
PUBLIC volta

SECTION TEXT
INPUT a
jmp mod_b
volta: STOP

SECTION DATA
a: SPACE
