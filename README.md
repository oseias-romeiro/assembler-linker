# macro-montador
Projeto de Software Basico que consiste em implementar um macro-montador em C/C++

## Descrição
<!--
    descrever detalhes do projeto
-->

## Compilar / Executar
Execute os comandos na pasta raiz do projeto

### Montador

```sh
make clean
./montador {file_name}
```

- `file_name`: nome do arquivo de entrada (sem extensão)
- saida: erros de compilação se ouver
- arquivos gerados: 
    - programa pre-processado `{file_name}_pre.asm`
    - código maquina `{file_name}.exc`

### Ligador
<!--
    como executar o ligador e detalhes de I/O
-->
