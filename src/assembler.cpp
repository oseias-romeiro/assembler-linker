#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

int wordCount = 0;

// Symbol Table (symbol, value)
unordered_map<string, int> symbolTable;


// get tokens of line
vector<string> getTokens(const string& line) {
    vector<string> tokens;
    istringstream iss(line);
    string token;

    // Verifica se há um rótulo no início da linha
    if (line.find(':') != string::npos) {
        iss >> token;
        wordCount++;
        symbolTable[token.substr(0, token.length() - 1)] = wordCount;
    }

    // Lê os demais tokens da linha
    while (iss >> token) {
        tokens.push_back(token);
        wordCount++;
    }

    return tokens;
}

void assembler(string pre_file) {
    ifstream sourceCodeFile(pre_file);

    if (!sourceCodeFile.is_open()) {
        throw "Não foi possível abrir o código pre-processado.";
    }

    string line;
    vector<string> tokens;
    while (getline(sourceCodeFile, line)) {

        tokens = getTokens(line);

    }
    sourceCodeFile.close();
}

