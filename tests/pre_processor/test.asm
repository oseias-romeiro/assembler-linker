; baseado no tests/arithmetic/add2.asm, poremn abusando das propriedades do pre-processador


SECTION TEXT
	INPUT in
	LOAD in
	ADD dois
	STORE res
	OUTPUT res



SECTION DATA
	dois:
		CONST 0x02 ; constante 2 em hexadecimal
	in: SPACE
	res: SPACE

