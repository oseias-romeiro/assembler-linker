# macro-montador
Projeto de Software Basico que consiste em implementar um macro-montador em C/C++

## Ambiente
Implementação de um montador e ligador implementado em C++, compilado em GCC e utilizando ambiente Linux (gcc 12.2.0-17ubuntu1). Desenvolvido com alvo em uma máquina hipotética e um assembly inventado para fins educativos utilizado durante a disciplina, e utilizando o `simulador` fornecido no trabalho para execução dos programas.

## Descrição
No projeto, foi preferível criar um arquivo pre-processado para cada programa e utilizar o algoritmo de passagem única no processo de montagem. Sendo o montador também responsável por apontar os erros encontrados e indicar o tipo de erro (léxico, sintático ou semântico) nos arquivos pre-processados. O ligador, recebe os arquivos objetos gerado pelo montador e gera o executável (caso não esteja usando módulos, o montador gera o executável diretamente).

## Testes
Na pasta `tests` contem códigos assembly com o intuito de testar o funcionamento do montador e do ligador e verificar se condiz com os requisistos do projeto. Segue abaixo a estrutura de pastas de testes:

```
├── addressing
├── errors
├── forwarding
├── loops
```


## Compilar / Execução
Execute os comandos na pasta raiz do projeto

### Gera os executáveis `montador` e `linker`

```sh
make all clean
```

### Montador

```sh
./montador {file_name}
```

- `file_name`: nome do arquivo assembly (sem extensão), podendo ser passado até 4 programas (módulos)
- saída: se houver, printa erros no formato: `<filename> - <numero_linha>: <menssagem_erro>`
- arquivos gerados: 
    - programa pre-processado `{file_name}_pre.asm`
    - Código:
        - Caso seja passado apenas um programa, é gerado código máquina `{file_name}.exc`
        - Caso use módulos, é gerado código objeto `{file_name}.obj`

### Ligador (linker)

```sh
./linker {file_name1} {file_name2}
```

- `file_name{i}`: nome do arquivo objeto (sem extensão), podendo ser passado até 4 módulos
- Gera o código máquina `{file_name1}.exc`


## Conclusão
Assim, foi implementado o montador/ligador satisfazendo os requisitos do projeto, sendo testado e verificado. Quero, além disso, relatar as dificulades na implementação: como lidar com forwarding-problem, endereçamento e deslocamento de endereços. Gerando uma dificuldade, maior que eu esperava, para conseguir resolve-los. Além do material de exemplo do projeto que não ajudou muito, pois não seguia a mesma sintáxe usada em sala de aula e os arquivos não se relacionavam, como o arquivo-objeto que não foi gerado apartir do código assembly mostrado. Porém, apesar das dificuldades, o projeto foi concluido com êxito, colocando em prática o conteúdo ministrado.

