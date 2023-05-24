#include <iostream>
#include <fstream>

#include <instructionTable.hpp>
#include <preProcessor.hpp>

using namespace std;

// TODO: como o método utilizado será passagem unica então tente juntar as funções de leitura e escrita com o assemble

// get source code
vector<string> getSourceCode(string pre_file){
    ifstream sourceCodeFile(pre_file);

    if (!sourceCodeFile.is_open()) {
        throw "Não foi possível abrir o código fonte.";
    }

    vector<string> sourceCode;
    string linha;
    while (getline(sourceCodeFile, linha)) {
        sourceCode.push_back(linha);
    }
    sourceCodeFile.close();

    return sourceCode;
}

// assemble source code to excutable code
//vector<int> assemble(const vector<string>& sourceCode) {}

void output(string file_path, vector<int> asmCode) {
    ofstream outFile(file_path + ".exc");

    if (!outFile.is_open()) {
        throw "Não foi possível criar o código objeto.";
    }

    for (int instruction : asmCode) {
        outFile << instruction << " ";
    }
    
    outFile.close();
}

int main(int argc, char* argv[]){
    vector<string> sourceCode;
    vector<int> asmCode;
    string pre_file;
    string file_source = (string) argv[1];

    // pre-processor
    pre_file = preProcessor(file_source);

    // get source code
    sourceCode = getSourceCode(pre_file);
    
    /*
    // assemble
    asmCode = assemble(sourceCode);

    // output file
    output(file_source, asmCode);

    */

    return 0;
}
