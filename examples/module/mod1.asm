; ler o valor 'a' e chama mod2
mod1: BEGIN
EXTERN: mod1
PUBLIC a

SECTION TEXT
INPUT a
jmp mod2
end

SECTION DATA
SPACE a