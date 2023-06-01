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

    if (argc > 5) {
        cout << "Quantidade de arquivos não suportado!" << endl;
    }else {
        for (int i = 1; i < argc; i++) {
            string file_source = (string) argv[i];

            // pre-processor
            preProcessor(file_source);

            //assembler
            assembler(file_source);
        }
    }
    return 0;
}
