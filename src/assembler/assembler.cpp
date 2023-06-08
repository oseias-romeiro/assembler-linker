#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <regex>

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
regex varRegex("[^a-zA-Z0-9_]");

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

string getLabel(string& tokens0) {
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
        else raiseError("Erro de sintaxe: construção inválida de seção");
        return true;// ignore line
    }
    return false;
}

void validLabel(string& variable){
    if (isdigit(variable[0]) || regex_search(variable, varRegex))
        raiseError("Erro léxico: declaração inválida");
}

int string2int(string value){
    int res=0;
    try {
        res = stoi(value);
    }
    catch(const std::exception& e) {
        raiseError("Erro de semântica: não foi possivel tratar o valor");
    }
    return res;
}

void assembler(ifstream& inFile, ofstream& outFile, bool gen_cod_objeto, string* pre_filename) {
    string line, value, label;
    vector<string> tokens, textCode, dataCode;
    Instruction inst;
    bool section_line=false, section_text_found=false, begin=false;
    regex twoLabels(":.*:");

    // init global variables
    addr = -1;
    section = SECTION_MOD;// 0: module; 1: text; 2: data
    lineCount = 0;
    filename = *pre_filename+"_pre.asm";
    symbolTable = {}, useTable = {};
    relVec = {};

    while (getline(inFile, line)) {
        lineCount++;
        tokens.clear();
        bool operAddr = false; // adress operations (ex.: label+2)
        
        // check uniqueless label
        if (regex_search(line, twoLabels)) raiseError("Erro de sintaxe: mais de uma label declarado na mesma linha");

        // line tokens
        tokens = getTokens(&line);
        if (tokens.size() > 0 && tokens.at(0) != "END") {
        // return label or ""
        label = getLabel(tokens.at(0));
        // set section scope
        if (tokens.size() >= 2)
            section_line = getSection(&tokens.at(0), &tokens.at(1));
        
        if (!section_line){
        if (label == "") {// no label
            switch (section) {
            case SECTION_MOD:
                if (tokens.at(0) == "PUBLIC"){
                    if(!begin) raiseError("Erro de semântica: esperado declaração de BEGIN");
                    symbolTable[tokens.at(1)] = {-1, false, true};
                }
                break;
            case SECTION_TEXT:
                section_text_found = true;
                // Instruction
                inst = instructionTable[tokens.at(0)];

                if (inst.opcodeNum) {
                    if ((int) tokens.size() != inst.wordSize){
                        if (tokens.size() == 4 && tokens.at(2) == "+") operAddr = true;
                        else raiseError("Erro de sintaxe: quantidade de operadores inesperado");
                    }

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
                                if(operAddr) textCode.push_back(to_string(string2int(EXTERNAL)+string2int(tokens.at(i+2))));
                                else textCode.push_back(EXTERNAL);// external is 0
                            else if (symbolTable[tokens.at(i)].value == -1)
                                if(operAddr) textCode.push_back("+ "+tokens.at(i)+" "+tokens.at(i+2));
                                else textCode.push_back(tokens.at(i));// PUBLIC declared but not know addres yet
                            else //known
                                if(operAddr) {
                                    try { textCode.push_back(to_string(symbolTable[tokens.at(i)].value + string2int(tokens.at(i+2)))); }
                                    catch(const std::exception& e) { raiseError("Erro de sintaxe após o operador '+'"); }
                                }
                                else textCode.push_back(to_string(symbolTable[tokens.at(i)].value));
                        }
                        else // token not known
                            if(operAddr) textCode.push_back("+ "+tokens.at(i)+" "+tokens.at(i+2));
                            else textCode.push_back(tokens.at(i));
                    }
                }
                else raiseError("Erro de semântica: instrução não encontrada");
                break;
            case SECTION_DATA:
                // section data should have label
                raiseError("Erro de semântica: nenhum rótulo encontrado na seção de dados");
                break;
            default:
                raiseError("Erro de semântica: tipo de sessão não reconhecida");
                break;
            }
        }else {// has label

            switch (section) {
            case SECTION_MOD:
                if (tokens.at(1) == "EXTERN") {
                    if(!begin) raiseError("Erro de semântica: esperado declaração de BEGIN");
                    symbolTable[label] = {0, true, false};
                    useTable[label] = {};
                }
                else if (tokens.at(1) == "BEGIN") {
                    begin=true;
                    symbolTable[label] = {0, false, true};
                }
                break;
            case SECTION_TEXT:
                section_text_found = true;

                // valid label
                validLabel(label);

                // declared label
                if (symbolTable.find(label) == symbolTable.end())
                    symbolTable[label] = {addr+1, false, false};// new label
                else symbolTable[label] = {addr+1, false, symbolTable[label].pub}; // update address

                // Instruction
                inst = instructionTable[tokens.at(1)];

                if (inst.opcodeNum) {
                    if ((int) tokens.size()-1 != inst.wordSize && tokens.at(3) != "+"){
                        if (tokens.at(3) == "+") operAddr= true;
                        else raiseError("Erro de sintaxe: quantidade de operadores inesperado");
                    }
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
                }else raiseError("Erro de semântica: instrução não encontrada");
                break;
            case SECTION_DATA:
                addr++;
                // valid label
                validLabel(label);
                // declared label
                if (symbolTable.find(label) == symbolTable.end())
                    symbolTable[label] = {addr, false, false};// new label
                else symbolTable[label] = {addr, false, symbolTable[label].pub}; // update address
                
                if (tokens.at(1) == "CONST") {
                    dataCode.push_back(tokens.at(2));
                } else if (tokens.at(1) == "SPACE") {
                    // check spaces
                    if(tokens.size() == 3)
                        for (int i = 0; i < string2int(tokens.at(2)); i++)
                            dataCode.push_back(SPACE);
                    else
                        dataCode.push_back(SPACE);
                } else raiseError("Erro de semântica: declarativa não reconhecida");
                
                break;
            default:
                // section data should have label
                raiseError("Erro de semântica: tipo de sessão não reconhecida");
                break;
            }

        }}}
    }
    if (!section_text_found) raiseError("Erro de semântica: Seção TEXT não encontrada");

    if (gen_cod_objeto){
        // USE
        outFile << "USO" << endl;
        for (const auto& use : useTable)
            for (size_t i = 0; i < use.second.size(); i++)
                outFile << use.first << " " << use.second.at(i) << endl;
        
        // DEF
        outFile << "DEF" << endl;
        for (const auto& sym : symbolTable){
            if (sym.second.value == -1) outFile << sym.first << " " << 0 << endl;
            else if (sym.second.pub) outFile << sym.first << " " << sym.second.value << endl;
        }
        // REL
        outFile << "RELATIVOS" << endl;
        for (size_t i = 0; i < relVec.size(); i++) {
            outFile << relVec.at(i) << " ";
        }
        outFile << endl;
        outFile << "CODE" << endl;
    }
    // forwarding problem
    for (size_t i = 0; i < textCode.size(); i++){
        value = textCode.at(i);
        if (!isdigit(value[0])){
            if (value[0] == '+') {
                vector<string> opers;
                istringstream iss(value);
                string plus, key, value;
                while (iss >> plus >> key >> value) {
                    if(symbolTable.find(key) != symbolTable.end())
                        outFile << symbolTable[key].value + string2int(value) << " ";
                    else
                        raiseError("Erro de semântica: símbolo não definidio");
                }
            }
            else{
                if(symbolTable.find(value) != symbolTable.end())
                    outFile << symbolTable[value].value << " ";
                else
                    raiseError("Erro de semântica: símbolo não definidio");
            }
        }
        else outFile << value << " ";
    }
    // data section at end
    for (size_t i = 0; i < dataCode.size(); i++){
        value = dataCode.at(i);
        if (!isdigit(value[0])) {
            if(symbolTable.find(value) != symbolTable.end())
                outFile << symbolTable[value].value << " ";
            else
                raiseError("Erro de semântica: símbolo não definidio");
        }
        else outFile << value << " ";
    }
}

