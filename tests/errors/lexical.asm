; exemplo de código com erros léxicos

SECTION TEXT
	OUTPUT _dois_2
	STOP

SECTION DATA
	_dois_2:    CONST 2; aceito
	2dois:      CONST 2; erro
	d@is:       CONST 2; erro
