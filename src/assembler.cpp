#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

#include <instructionTable.hpp>

using namespace std;

int addr = 0, lineCount = 0;
bool section_data = false, section_text = true;

// Symbol Table (symbol, value)
unordered_map<string, int> symbolTable;


// get tokens of line
// if section_data feed symbolTable
vector<string> getTokens(const string line) {
    vector<string> tokens;
    istringstream iss(line);
    string token;
    
    // get label in line
    if (line.find(':') != string::npos) {
        iss >> token;
        symbolTable[token.substr(0, token.length() - 1)] = addr;
    }

    // get line tokens
    while (iss >> token) {
        if (token != ",") {// comma ignore
            tokens.push_back(token);
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

        // line tokens
        tokens = getTokens(line);

        /* outputs apenas para testes */
        cout << "line: " << line << endl;
        cout << "tokens: ";
        for (size_t i = 0; i < tokens.size(); i++)
        {
            cout << tokens.at(i) << " ";
        }
        cout << endl;
        cout << "address: " << addr << endl;
        

        if (tokens.size() > 0) {

            // check section scope
            if (tokens.at(0) == "SECTION"){
                if (tokens.at(1) == "DATA") { section_data=true; section_text=false; }
                else if (tokens.at(1) == "TEXT") {section_data=false; section_text=true;}
                else { cout << "section erro, linha: " << lineCount << endl; }
            }else {
                if (section_text){
                    // Instruction
                    inst = instructionTable[tokens.at(0)];

                    if (inst.opcodeSim != ""){
                        // write opcode
                        excCodeFile << inst.opcodeNum << " ";

                        // write operators
                        for (int i = 1; i < inst.wordSize; i++) {
                            excCodeFile << symbolTable[tokens.at(i)] << " ";
                        }
                    }else {
                        cout << "(linha " << lineCount << ") instrução não encontrada" << endl;
                    }
                    addr += tokens.size();
                } else if (section_data){
                    if (tokens.at(0) == "CONST") {
                        // TODO: tratar constantes (converter para decimal)
                        excCodeFile << tokens.at(1) << " ";
                    } else if (tokens.at(0) == "SPACE") {
                        excCodeFile << "00 ";
                    } else {
                        cout << "(linha " << lineCount << ") erro na diretiva de dados" << endl;
                    }
                    addr++;
                }
            }
        }
        tokens.clear();
    }
    sourceCodeFile.close();
    excCodeFile.close();
}

