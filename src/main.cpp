#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <preProcessor.hpp>
#include <assembler.hpp>

using namespace std;

int main(int argc, char* argv[]){
    vector<string> sourceCode;
    vector<int> asmCode;
    string file_source = (string) argv[1];

    // pre-processor
    preProcessor(file_source);

    //assembler
    assembler(file_source);

    return 0;
}
