#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

#define USO 0
#define DEF 1
#define RELATIVOS 2
#define CODE 3

// global tables
vector<int> code;
vector<pair<int, int>> relatives;
vector<pair<string, int>> uso;
unordered_map<string, int> def;
// global variables
vector<int> correctionFactor;
int fileId = 0;
int section; // 0: uso; 1: def; 2: relativos; 3: code
int addr = -1;

void joinGlobalTables(const string* line){
	istringstream iss(*line);
	string value, key;

	// fill tables
	if (section == USO)
		while (iss >> key >> value){
			uso.push_back(pair(key, stoi(value) + correctionFactor.at(fileId)));
		}
	else if (section == DEF)
		while (iss >> key >> value){
			def[key] = stoi(value) + correctionFactor.at(fileId);
		}
	else if (section == RELATIVOS) 
		while (iss >> value){
			try {
				relatives.push_back(pair(
					stoi(value),
					correctionFactor.at(fileId)
				));
			}
			catch(const std::exception& e){}
		}
	else if (section == CODE)
		while (iss >> value){
			try {
				code.push_back(stoi(value));
				addr++;
			}
			catch(const std::exception& e){}
		}
}
void correction(){
	bool corrected;
	int addr_c;

	for (auto &&r : relatives){
		corrected = false;
		addr_c = r.first+r.second;
		// global correction
		for (auto &&u : uso)
			if (u.second == addr_c){
				// get in def
				code[addr_c] += def[u.first];// if not 0 so has address opdration
				corrected = true; //mark as corrected
			}
		// local correction
		if (!corrected) code[addr_c] += r.second;
	}
}

void readFile(const string filename){
	ifstream objCode(filename+".obj");
	string line;

	// update correctionFactor
	correctionFactor.push_back(addr+1);
	
	if (!objCode.is_open())
        cout << "Não foi possível encontrar o arquivo" << filename << ".obj" << endl;

	while (getline(objCode, line)) {
		// join to global tables
		joinGlobalTables(&line);
		// get section
		if (line.find("USO")  != string::npos) section = USO;
		else if (line.find("DEF")  != string::npos) section = DEF;
		else if (line.find("RELATIVOS")  != string::npos) section = RELATIVOS;
		else if (line.find("CODE") != string::npos) section = CODE;
	}
	objCode.close();
}

int main(int argc, char* argv[]){

    if (argc > 5) cout << "Quantidade de arquivos não suportado!" << endl;

	string filename = argv[1];
	ofstream output(filename + ".exc");

	if (!output.is_open())
        cout << "Não foi possível criar o arquivo de saida" << endl;

	// read file and resolve global tables
	for (int i = 1; i < argc; i++) {
		fileId = i-1;
		section = -1;
		readFile(argv[i]);
	}
	// correcion adresses
	correction();

	// write output
	for (size_t i = 0; i < code.size(); i++)
		output << code.at(i) << " ";
	
	output.close();
    return 0;
}
