#include <iostream>
#include <fstream>

#include <instructionTable.h>

using namespace std;


vector<string> getSourceCode(string file_path){
    // get source code
    ifstream sourceCodeFile(file_path + ".asm");

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

vector<int> assemble(const vector<string>& codigoFonte) {}

void output(string file_path, vector<int> asmCode) {
    ofstream outFile(file_path);

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

    // get source code
    try { sourceCode = getSourceCode((string) argv[1]); }
    catch(char const* e){ cerr << e << endl; return 1;}
    
    // assemble
    asmCode = assemble(sourceCode);

    // output file
    try { output((string) argv[1], asmCode); }
    catch(char const* e){ cerr << e << endl; return 1;}

    return 0;
}
