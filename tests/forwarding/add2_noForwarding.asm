; estutura simples sem forwarding problem
; programa soma 2 e mostra o resultado na tela

SECTION DATA
dois: CONST 2
in: SPACE
res: SPACE

SECTION TEXT
INPUT in
LOAD in
ADD dois
STORE res
OUTPUT res
STOP
