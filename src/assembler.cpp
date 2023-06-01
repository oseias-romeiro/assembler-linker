#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

#include <instructionTable.hpp>

using namespace std;

int addr = -1, lineCount = 0;
bool section_data = false, section_text = true;

// Symbol Table (symbol, value, external)
unordered_map<string, pair<int, bool>> symbolTable;
// Definition Table
unordered_map<string, int> defTable;
// Use Table
unordered_map<string, vector<int>> useTable;

// get tokens of line
// if section_data feed symbolTable
vector<string> getTokens(const string line) {
    vector<string> tokens;
    istringstream iss(line);
    string token;
    bool use=false, def=false;
    
    // get label in line
    if (line.find(':') != string::npos) {
        iss >> token;
        token = token.substr(0, token.length() - 1);

        // public var in data secion
        if (section_data)
            if (defTable.find(token) != defTable.end()) defTable[token] = addr;

        if (token == "PUBLIC") {
            def = true;
            symbolTable[token] = make_pair(addr, false);
        }else if (token == "EXTERN") {
            use = true;
            symbolTable[token] = make_pair(addr, true);
        }else {
            symbolTable[token] = make_pair(addr, false);
        }
    }

    // get line tokens
    while (iss >> token) {
        if (use) useTable[token] = {};
        else if (def) defTable[token] = 00;
        else {
            if (token != ",") tokens.push_back(token); // comma ignore
        }
    }
    return tokens;
}

// TODO: lidar com módulos
// TODO: geração de codigo objeto
void assembler(const string file_source) {
    ifstream sourceCodeFile(file_source+"_pre.asm");
    ofstream excCodeFile(file_source+".obj");

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
        for (size_t i = 0; i < tokens.size(); i++) {
            cout << tokens.at(i) << " ";
        }
        cout << endl;
        cout << "address: " << addr << endl;
        

        if (tokens.size() > 0) {

            // check section scope
            if (tokens.at(0) == "SECTION") {
                if (tokens.at(1) == "DATA") { section_data=true; section_text=false; }
                else if (tokens.at(1) == "TEXT") {section_data=false; section_text=true;}
                else cout << "section syntax erro, linha: " << lineCount << endl;
            }else {
                if (section_text){
                    // Instruction
                    inst = instructionTable[tokens.at(0)];

                    if (inst.opcodeSim != ""){
                        // write opcode
                        excString.push_back(to_string(inst.opcodeNum));

                        // write operators
                        for (int i = 1; i < inst.wordSize; i++) {
                            // public var declaration
                            if (defTable.find(tokens.at(i)) != defTable.end()) defTable[tokens.at(i)] = addr;
                            // extern var used
                            if (useTable.find(tokens.at(i)) != useTable.end()) useTable[tokens.at(i)].push_back(addr);

                            // use symbol
                            if (symbolTable.find(tokens.at(i)) != symbolTable.end())
                                excString.push_back(to_string(symbolTable[tokens.at(i)].first));
                            else excString.push_back(tokens.at(i));
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
                    } else cout << "(linha " << lineCount << ") erro na diretiva de dados" << endl;
                    addr++;
                }
            }
        }
    }

    // USE
    excCodeFile << "USO" << endl;
    for (const auto& use : useTable)
        for (size_t i = 0; i < use.second.size(); i++)
            excCodeFile << use.first << " " << use.second.at(i) << endl;
    
    // DEF
    excCodeFile << "DEF" << endl;
    for (const auto& def : defTable)
        excCodeFile << def.first << " " << def.second << endl;

    // TODO: RELATIVOS
    // REL
    excCodeFile << "RELATIVOS" << endl;

    // TODO: fazer essa parte dentro do loop de leitura
    // forwarding problem
    excCodeFile << "CODE" << endl;
    for (size_t i = 0; i < excString.size(); i++){
        value = excString.at(i);
        if (!isdigit(value[0])) excCodeFile <<  symbolTable[value].first << " ";
        else excCodeFile << value << " ";
    }
    sourceCodeFile.close();
    excCodeFile.close();
}

