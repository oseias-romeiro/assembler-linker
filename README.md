# macro-montador
Projeto de Software Basico que consiste em implementar um macro-montador em C/C++

# Descrição
<!--
    descrever detalhes do projeto
-->

# Compilar/Executar
Execute os comandos na pasta raiz do projeto

## Montador

```sh
    g++ -I ./includes -Wall -o montador ./src/montador.cpp
    ./montador {{ assembly_file_name }}
```

- `assembly_file_name`: nome do código fonte em assembly (sem extensão)
- saída: é gerado o código executável (.exc), e se houver, erros são mostrados na tela

## Ligador
<!--
    como executar o ligador e detalhes I/O
-->
