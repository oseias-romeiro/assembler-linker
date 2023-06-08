#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <stdexcept>

using namespace std;

void assembler(ifstream& inFile, ofstream& outFile, bool gen_cod_objeto, string* pre_filename);

#endif
