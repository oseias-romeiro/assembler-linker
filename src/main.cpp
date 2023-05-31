#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <preProcessor.hpp>
#include <assembler.hpp>

#define ARGS_LIMIT 4 // assembly files limit

using namespace std;

int main(int argc, char* argv[]){
    vector<string> sourceCode;
    vector<int> asmCode;

    for (int i = 1; i < ARGS_LIMIT; i++) {
        string file_source = (string) argv[i];

        if (file_source != "") {
            // pre-processor
            preProcessor(file_source);

            //assembler
            assembler(file_source);
        }   
    }

    return 0;
}
