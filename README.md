# macro-montador
Projeto de Software Basico que consiste em implementar um macro-montador em C/C++

## Descrição
Implementação de um montador e ligador implementado em C/C++ e compilado em GCC. Desenvolvido com alvo em uma máquina hipotética e um assembly inventado, utilizado durante a disciplina.

## Testes
Testes foram desenvolvidos para verificar o funcionado do montador e do ligador, disponibilizados na pasta `tests`

## Compilar / Execução
Execute os comandos na pasta raiz do projeto

```sh
make all clean
```

- Gera os executáveis `montador` e `linker`

### Montador

```sh
./montador {file_name}
```

- `file_name`: nome do arquivo assembly (sem extensão), podendo ser passado até 4 programas (módulos)
- saida: erros, se ouver
- arquivos gerados: 
    - programa pre-processado `{file_name}_pre.asm`
    - Código:
        - caso seja passado apenas um programa, é gerado código máquina `{file_name}.exc`
        - Caso use módulos, é gerado código objeto `{file_name}.obj`

### Ligador

```sh
./linker {file_name1} {file_name2}
```

- `file_namei`: nome do arquivo objeto (sem extensão), podendo ser passado até 4 módulos
- Gera o código máquina `{file_name1}.exc`

