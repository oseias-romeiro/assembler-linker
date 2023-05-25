/* Tarefas do pre-processador

- transforma tudo para maiusculo
- retira espaços, tabulações ou enter desnecessários
- qualquer rótulo (label + ':') seguido de enter deve ser colocado em uma linha só
- retirar comentários (linha começa com ';')
- converter valores depois de "CONST" para decimais

*/

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

void preProcessor(const string inputFile) {
    ifstream inFile(inputFile + ".asm");
    ofstream outFile(inputFile + "_pre.asm");

    if (!inFile.is_open() || !outFile.is_open()) {
        throw "Erro ao abrir os arquivos.";
    }

    string line;
    while (getline(inFile, line)) {
        
        // ignora comentários (linhas começando com ';')
        if (line.find(";")) {

            // Transforma tudo em maiúsculas
            transform(line.begin(), line.end(), line.begin(), ::toupper);

            // TODO: Remove espaços, tabulações e enters desnecessários

            // TODO: Rotulos com valores na mesma linha

            // TODO: Converte valores depois de "CONST" para decimais

            outFile << line << "\n";
        }
    }
    inFile.close();
    outFile.close();
}
