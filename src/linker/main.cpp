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
unordered_map<string, int> uso, def;
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
			uso[key] = stoi(value) + correctionFactor.at(fileId);
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
	for (size_t i = 0; i < code.size(); i++){
		corrected = false;
		for (auto &&u : uso){
			// need correction
			if (u.second == (int) i){
				// search in def table
				if (def.find(u.first) != def.end()){
					cout << "uso: " << code[i] << "->" << def[u.first] << endl;
					code[i] = def[u.first];
					corrected = true;
				}
			}
		}
		if (!corrected)
			for (size_t j = 0; j < relatives.size(); j++){
				// need correction
				if (relatives[j].first+relatives[j].second == (int) i) {
					cout << "rel: " << code[i] << "->" << code[i]+relatives[j].second << endl;
					code[i] = code[i]+relatives[j].second;
				}
			}
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
