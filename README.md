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
make all clean
./montador {file_name}
```

- `file_name`: nome do arquivo de entrada (sem extensão), podendo ser passado até 4 programas (módulos)
- saida: erros, se ouver
- arquivos gerados: 
    - programa pre-processado `{file_name}_pre.asm`
    - um programa: código maquina `{file_name}.exc`
    - módulos: código objeto `{file_name}.obj`

### Ligador
<!--
    como executar o ligador e detalhes de I/O
-->
