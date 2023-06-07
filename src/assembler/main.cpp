#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#include <preProcessor.hpp>
#include <assembler.hpp>

using namespace std;

int main(int argc, char* argv[]){

    if (argc > 5) throw runtime_error("Quantidade de arquivos não suportado!");
    else 
        for (int i = 1; i < argc; i++) {
            // pre-processor
            preProcessor(string(argv[i]));

            //assembler
            assembler(string(argv[i]), !(argc==2));
        }
    return 0;
}
