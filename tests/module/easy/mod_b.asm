; adiciona 2 na variavel externa 'a', salva o resultado em tmp, mostra na tela e retorna para mod_a
mod_b: BEGIN
a: EXTERN
volta: EXTERN

SECTION TEXT
LOAD a
ADD dois
STORE tmp
OUTPUT tmp
jmp volta

SECTION DATA
dois: CONST 2
tmp: SPACE
