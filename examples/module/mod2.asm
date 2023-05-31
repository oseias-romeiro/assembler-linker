; subtrai 2 da variavel externa 'a', mostra na tela e retorna para mod1
mod2: BEGIN
EXTERN: a
EXTERN: mod1

SECTION TEXT
SUB dois
STORE tmp
OUTPUT tmp
jmp mod1

SECTION DATA
CONST dois 2
SPACE tmp