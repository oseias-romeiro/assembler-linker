#include <iostream>
#include <fstream>

#include <instructionTable.hpp>
#include <preProcessor.hpp>
#include <assembler.hpp>

using namespace std;

int main(int argc, char* argv[]){
    vector<string> sourceCode;
    vector<int> asmCode;
    string pre_file;
    string file_source = (string) argv[1];

    // pre-processor
    pre_file = preProcessor(file_source);

    //assembler
    assembler(pre_file);

    return 0;
}
