; exemplo de código com erros semanticos

one: 	EXTERN 	; deveria ser declarado depois de um BEGIN
PUBLIC 	dois 	; deveria ser declarado depois de um BEGIN

SECTION TEXT 	; se comentar essa linha -> gera o erro e não reconhece as instruções abaixo
	JUMP nowhere; instrução não encontrada
	JMP nowhere ; dado não definido
	STOP
