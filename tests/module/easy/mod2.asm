; adiciona 2 na variavel externa 'a', salva o resultado em tmp, mostra na tela e retorna para mod1
mod2: BEGIN
EXTERN: a
EXTERN: volta

SECTION TEXT
LOAD a
ADD dois
STORE tmp
OUTPUT tmp
jmp volta

SECTION DATA
dois: CONST 2
tmp: SPACE
