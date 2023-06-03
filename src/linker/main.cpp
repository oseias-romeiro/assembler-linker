#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

#define RELATIVOS 0
#define CODE 1

// global tables
vector<string> code, relatives;
// global variables
int correctionFactor = 0;
int section; // 0: relativos; 1: code
int addr = -1;

void joinGlobalTables(const string* line){
	istringstream iss(*line);
	string value;

	while (iss >> value){
		// add relativos
		if (section == RELATIVOS) relatives.push_back(value);
		// add code
		if (section == CODE){
			code.push_back(value);
			addr++;
		}
	}
}
void correctRelatives(){
	for (size_t i = 0; i < code.size(); i++)
		for (size_t j = 0; j < relatives.size(); j++)
			if (code.at(i) == relatives.at(j))
				code[i] = to_string(stoi(code[i])+correctionFactor);
}

void readFile(const string filename){
	ifstream objCode(filename+".obj");
	string line;
	section = -1;
	
	if (!objCode.is_open())
        cout << "Não foi possível encontrar o arquivo" << filename << ".obj" << endl;

	while (getline(objCode, line)) {
		// join to global tables
		joinGlobalTables(&line);

		// get section
		if (line.find("CODE") != string::npos) section = CODE;
		else if (line.find("RELATIVOS")  != string::npos) section = RELATIVOS;
	}
	objCode.close();

	// correcion relatives
	correctRelatives();
	// update correctionFactor
	correctionFactor = addr;
}

int main(int argc, char* argv[]){

    if (argc > 5) cout << "Quantidade de arquivos não suportado!" << endl;

	string filename = argv[1];
	ofstream output(filename + ".exc");

	if (!output.is_open())
        cout << "Não foi possível criar o arquivo de saida" << endl;

	// read file and resolve global tables
	for (int i = 1; i < argc; i++) readFile(argv[i]);
	
	// write output
	for (size_t i = 0; i < code.size(); i++)
		output << code.at(i) << " ";
	
	output.close();
    return 0;
}
