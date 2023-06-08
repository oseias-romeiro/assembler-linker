; recebe 2 valores como entrada e armazena em data e data+1
; depois soma e mostra o resultado na tela

SECTION TEXT
	INPUT 	data
	INPUT 	data + 1
	LOAD 	data
	ADD 	data + 1
	STORE 	res
	OUTPUT 	res
	STOP

SECTION DATA
	res: SPACE
	data: SPACE 2
