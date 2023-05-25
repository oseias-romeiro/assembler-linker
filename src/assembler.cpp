#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

#include <instructionTable.hpp>

using namespace std;

int wordCount = 0, lineCount = 0; 

// Symbol Table (symbol, value)
unordered_map<string, int> symbolTable;


// get tokens of line
vector<string> getTokens(const string line) {
    vector<string> tokens;
    istringstream iss(line);
    string token;
    
    // Verifica se há um rótulo no início da linha
    if (line.find(':') != string::npos) {
        iss >> token;
        symbolTable[token.substr(0, token.length() - 1)] = wordCount;
    }

    // Lê os demais tokens da linha
    while (iss >> token) {
        if (token != ",") {
            tokens.push_back(token);
            wordCount++;
        }
    }
    return tokens;
}

void assembler(const string file_source) {
    ifstream sourceCodeFile(file_source+"_pre.asm");
    ofstream excCodeFile(file_source+".exc");

    if (!sourceCodeFile.is_open()) {
        cout << "Não foi possível abrir o código pre-processado." << endl;
    }

    string line;
    vector<string> tokens;
    Instruction inst;
    while (getline(sourceCodeFile, line)) {
        lineCount++;
        
        cout << " line: " << line << endl;

        // line tokens
        tokens = getTokens(line);
        cout << " tokens: ";
        for (size_t i = 0; i < tokens.size(); i++)
        {
            cout << tokens.at(i) << " ";
        }
        cout << endl;
        cout << "wordCount: " << wordCount << endl;
        

        
        if (tokens.size() > 0) {
            // Instruction
            inst = instructionTable[tokens.at(0)];

            if (inst.opcodeSim != ""){
                // write opcode
                excCodeFile << inst.opcodeNum << " ";

                // write operators
                for (int i = 1; i < inst.wordSize; i++) {
                    excCodeFile << symbolTable[tokens.at(i)] << " ";
                }
            }
        }
        tokens.clear();
    }
    sourceCodeFile.close();
    excCodeFile.close();
}

