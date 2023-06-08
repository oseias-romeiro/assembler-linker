; exemplo de código com erros de sintáxe

SECTION TEXT
	OUTPUT dois dois ; quantidade de operadores inválido
	loop1: loop2: add dois ; conflito de rótulos
	STOP

SECTION DATA
	dois: CONST 2
