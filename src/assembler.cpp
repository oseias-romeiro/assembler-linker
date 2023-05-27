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

    string line, value;
    vector<string> tokens, excString;
    Instruction inst;
    while (getline(sourceCodeFile, line)) {
        lineCount++;
        tokens.clear();

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
                        excString.push_back(to_string(inst.opcodeNum));

                        // write operators
                        for (int i = 1; i < inst.wordSize; i++) {
                            if (symbolTable.find(tokens.at(i)) != symbolTable.end()){
                                excString.push_back(to_string(symbolTable[tokens.at(i)]));
                            }else {
                                excString.push_back("XX");
                            }
                        }
                    }else {
                        cout << "(linha " << lineCount << ") instrução não encontrada" << endl;
                    }
                    addr += tokens.size();
                } else if (section_data){
                    if (tokens.at(0) == "CONST") {
                        // TODO: tratar constantes (converter para decimal)
                        excString.push_back(tokens.at(1));
                    } else if (tokens.at(0) == "SPACE") {
                        // TODO: SPACE deve aceitar argumento
                        excString.push_back("00");
                    } else {
                        cout << "(linha " << lineCount << ") erro na diretiva de dados" << endl;
                    }
                    addr++;
                }
            }
        }
    }
    
    // forwarding problem
    for (size_t i = 0; i < excString.size(); i++){
        value = excString.at(i);
        if (value == "XX") {
            excCodeFile <<  symbolTable[value] << " ";
        }else {
            excCodeFile << value << " ";
        }
    }

    sourceCodeFile.close();
    excCodeFile.close();
}

