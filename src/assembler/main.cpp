#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#include <preProcessor.hpp>
#include <assembler.hpp>

using namespace std;

int main(int argc, char* argv[]){
    ifstream asmCode, preAsmCode;
    ofstream preCode, outCode;
    string filename;

    if (argc > 5) throw runtime_error("Quantidade de arquivos não suportado!");
    else 
        for (int i = 1; i < argc; i++) {
            filename = argv[i];
            // pre-processor
            asmCode = ifstream(filename+".asm");
            preCode = ofstream(filename+"_pre.asm");
            if (!asmCode.is_open() || !preCode.is_open()) throw fstream::failure("Pre-processador: Erro ao abrir/criar os arquivos.");
            preProcessor(asmCode, preCode);

            // Close stream files
            asmCode.close();
            preCode.close();

            //assembler
            preAsmCode = ifstream(filename+"_pre.asm");
            if (argc > 2) // object code
                outCode = ofstream(filename+".obj");
            else // machine code
                outCode = ofstream(filename+".exc");
            if (!preAsmCode.is_open() || !outCode.is_open()) throw fstream::failure("Montador: Erro ao abrir/criar os arquivos.");
            assembler(preAsmCode, outCode, (argc > 2), &filename);

            // Close stream files
            preAsmCode.close();
            outCode.close();
        }

    return 0;
}
