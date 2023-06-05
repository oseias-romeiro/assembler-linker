#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

#include <instructionTable.hpp>

// flags to section scope
#define SECTION_MOD  0
#define SECTION_TEXT 1
#define SECTION_DATA 2

#define EXTERNAL "0"
#define SPACE "0"

using namespace std;

// Symbol Table (symbol, value, external, public)
struct symbol {
    int value;
    bool external, pub;
};
unordered_map<string, symbol> symbolTable;
// Use Table
unordered_map<string, vector<int>> useTable;
// Relatives adresses vector
vector<int> relVec;

// global variables
int section, lineCount ,addr;
string filename;

// get line tokens
vector<string> getTokens(const string* line) {
    vector<string> tokens;
    istringstream iss(*line);
    string token;

    while (iss >> token) {
        if (token != ",") tokens.push_back(token); // comma ignore
    }
    return tokens;
}

string getLabel(string tokens0) {
    if (tokens0.find(':') != string::npos) return tokens0.substr(0, tokens0.length() - 1);
    else return "";
}

void raiseError(const char* msg) {
    cout << filename << " - " << lineCount <<": " << msg << endl;
}

bool getSection(const string* token0, const string* token1){
    if (*token0 == "SECTION") {
        if (*token1 == "TEXT") section = SECTION_TEXT;
        else if (*token1 == "DATA") section = SECTION_DATA;
        else raiseError("erro de sintaxe: construção invalida de seção na linha");
        return true;// ignore line
    }
    return false;
}

// TODO: lidar com módulos
// TODO: geração de codigo objeto/maquina
void assembler(const string file_source, bool gen_cod_objeto) {
    ifstream sourceCodeFile(file_source+"_pre.asm");
    ofstream excCodeFile;
    string line, value, label;
    vector<string> tokens, textCode, dataCode;
    Instruction inst;
    bool section_line = false;

    // init global variables
    addr = -1;
    section = SECTION_MOD;// 0: module; 1: text; 2: data
    lineCount = 0;
    filename = file_source+"_pre.asm";
    symbolTable = {}, useTable = {};
    relVec = {};

    // choose gen exc or obj code
    if (gen_cod_objeto) excCodeFile = ofstream(file_source+".obj");
    else excCodeFile = ofstream(file_source+".exc");

    if (!sourceCodeFile.is_open() || !excCodeFile.is_open())
        cout << "Não foi possível abrir o código pre-processado ou criar o arquivo de saida." << endl; // throw
    else {

    while (getline(sourceCodeFile, line)) {
        lineCount++;
        tokens.clear();

        // line tokens
        tokens = getTokens(&line);
        if (tokens.size() > 0) {
        // return label or ""
        label = getLabel(tokens.at(0));
        // set section scope
        if (tokens.size() >= 2)
            section_line = getSection(&tokens.at(0), &tokens.at(1));
        
        if (!section_line){
        if (label == "") {
            switch (section) {
            case SECTION_MOD:
                if (tokens.at(0) == "PUBLIC") symbolTable[tokens.at(1)] = {-1, false, true};
                break;
            case SECTION_TEXT:
                // Instruction
                inst = instructionTable[tokens.at(0)];

                // TODO: valid instruction
                if (inst.opcodeNum) {
                    if ((int) tokens.size() != inst.wordSize)
                        raiseError("Erro de sintaxe: quantidade de operadores inesperado");
                    // opcode
                    textCode.push_back(to_string(inst.opcodeNum));
                    addr++;
                    // operators
                    for (int i = 1/* ignore command */; i < inst.wordSize; i++) {
                        addr++;
                        // relatives
                        relVec.push_back(addr);
                        // extern var used (token_i in useTable)
                        if (useTable.find(tokens.at(i)) != useTable.end())
                            useTable[tokens.at(i)].push_back(addr);
                        // check token
                        if (symbolTable.find(tokens.at(i)) != symbolTable.end()) {
                            if (symbolTable[tokens.at(i)].external)
                                textCode.push_back(EXTERNAL);// external is 0
                            else if (symbolTable[tokens.at(i)].value == -1)
                                textCode.push_back(tokens.at(i));// PUBLIC declared but not know addres yet
                            else
                                textCode.push_back(tokens.at(i));// token not known
                        }
                        else textCode.push_back(tokens.at(i));// token not known
                    }
                }
                else raiseError("erro de sintaxe: instrução não encontrada");
                break;
            case SECTION_DATA:
                // section data should have label
                raiseError("erro de sintaxe");
                break;
            default:
                // section data should have label
                cout << "erro no escopo da seção " << endl;
                break;
            }
        }else {// has label

            switch (section) {
            case SECTION_MOD:
                if (tokens.at(1) == "EXTERN") {
                    symbolTable[label] = {0, true, false};
                    useTable[label] = {};
                }
                else if (tokens.at(1) == "BEGIN") symbolTable[label] = {0, false, true};
                break;
            case SECTION_TEXT:

                // declared label
                if (symbolTable.find(label) == symbolTable.end())
                    symbolTable[label] = {addr+1, false, false};// new label
                else symbolTable[label] = {addr+1, false, symbolTable[label].pub}; // update address

                // Instruction
                inst = instructionTable[tokens.at(1)];

                // TODO: valid instruction
                if (inst.opcodeNum) {
                    if ((int) tokens.size()-1 != inst.wordSize)
                        raiseError("Erro de sintaxe: quantidade de operadores inesperado");
                    // opcode
                    textCode.push_back(to_string(inst.opcodeNum));
                    addr++;
                    // operators
                    for (int i = 2/* ignore command & label */; i < inst.wordSize+1; i++) {
                        addr++;
                        // relatives
                        relVec.push_back(addr);
                        // extern var used (token_i in useTable)
                        if (useTable.find(tokens.at(i)) != useTable.end()) useTable[tokens.at(i)].push_back(addr);
                        // check token
                        if (symbolTable.find(tokens.at(i)) != symbolTable.end()) {
                            if (symbolTable[tokens.at(i)].external)
                                textCode.push_back(EXTERNAL);// external is 0
                            else if (symbolTable[tokens.at(i)].value == -1)
                                textCode.push_back(tokens.at(i));// PUBLIC declared but not know addres yet
                            else
                                textCode.push_back(tokens.at(i));// token not known
                        }
                        else textCode.push_back(tokens.at(i));// token not known
                    }
                }else raiseError("erro de sintaxe");
                break;
            case SECTION_DATA:
                addr++;

                // declared label
                if (symbolTable.find(label) == symbolTable.end())
                    symbolTable[label] = {addr, false, false};// new label
                else symbolTable[label] = {addr, false, symbolTable[label].pub}; // update address
                
                if (tokens.at(1) == "CONST") {
                    // TODO: tratar constantes (converter para decimal)
                    dataCode.push_back(tokens.at(2));
                } else if (tokens.at(1) == "SPACE") {
                    // TODO: SPACE deve aceitar argumento
                    dataCode.push_back(SPACE);
                } else raiseError("erro de sintaxe: declarativa não reconhecida");
                
                break;
            default:
                // section data should have label
                raiseError("erro no escopo da seção");
                break;
            }

        }}}
    }

    if (gen_cod_objeto){
        // USE
        excCodeFile << "USO" << endl;
        for (const auto& use : useTable)
            for (size_t i = 0; i < use.second.size(); i++)
                excCodeFile << use.first << " " << use.second.at(i) << endl;
        
        // DEF
        excCodeFile << "DEF" << endl;
        for (const auto& sym : symbolTable)
            if (sym.second.pub) excCodeFile << sym.first << " " << sym.second.value << endl;

        // TODO: RELATIVOS
        // REL
        excCodeFile << "RELATIVOS" << endl;
        for (size_t i = 0; i < relVec.size(); i++) {
            excCodeFile << relVec.at(i) << " ";
        }
        excCodeFile << endl;
        excCodeFile << "CODE" << endl;
    }
    // TODO: fazer essa parte dentro do loop de leitura
        // forwarding problem
    for (size_t i = 0; i < textCode.size(); i++){
        value = textCode.at(i);
        if (!isdigit(value[0])) excCodeFile <<  symbolTable[value].value << " ";
        else excCodeFile << value << " ";
    }
    for (size_t i = 0; i < dataCode.size(); i++){
        value = dataCode.at(i);
        if (!isdigit(value[0])) excCodeFile <<  symbolTable[value].value << " ";
        else excCodeFile << value << " ";
    }
    }
    sourceCodeFile.close();
    excCodeFile.close();
}

