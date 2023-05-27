#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct Instruction
{
    string opcodeSim;
    int opcodeNum;
    int wordSize;
};

unordered_map<string, Instruction> instructionTable = {
    {"ADD",{"ADD", 1, 2}},
    {"COPY",{"COPY", 9, 3}},
    {"DIV",{"DIV", 4, 2}},
    {"INPUT",{"INPUT", 12, 2}},
    {"JMP",{"JMP", 5, 2}},
    {"JMPN",{"JMPN", 6, 2}},
    {"JMPP",{"JMPP", 7, 2}},
    {"JMPZ",{"JMPZ", 8, 2}},
    {"LOAD",{"LOAD", 10, 2}},
    {"MUL",{"MUL", 3, 2}},
    {"OUTPUT",{"OUTPUT", 13, 2}},
    {"STOP",{"STOP", 14, 1}},
    {"STORE",{"STORE", 11, 2}},
    {"SUB",{"SUB", 2, 2}}
};

