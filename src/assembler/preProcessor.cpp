#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>

using namespace std;

string hex2decimal(string hex){
    int decimal = stoi(hex, nullptr, 16);
    return to_string(decimal);
}

string transformDec(string line, regex hexRegex){
    smatch match;
    string text="";
    regex_search(line, match, hexRegex);

    if (match.str() == "") return line;

    for (size_t i = 0; i < line.find("0X"); i++) text += line[i];
    return text+hex2decimal(match.str());
}

void preProcessor(ifstream& inFile, ofstream& outFile) {
    string line, nl;
    regex spacesRegex("\\s+"),
          tabsRegex("\\t"),
          space_nlRegex(":\\n"),
          hexRegex("(0X[0-9A-Fa-f]+)"),
          commentsRegex(";.*")
    ;

    while (getline(inFile, line)) {
        nl = "\n";
        if (line != "\n" && !line.empty()) {// ignora comentários e linhas vazias
            // transform to upper case
            transform(line.begin(), line.end(), line.begin(), ::toupper);
            // remove comments
            line = regex_replace(line, commentsRegex, "");
            // remove tabs
            line = regex_replace(line, tabsRegex, "");
            // multiple spaces to one space
            line = regex_replace(line, spacesRegex, " ");
            // labels in same line
            if(!line.empty() && line.back() == ':'){
                line += " ";
                nl = "";
            }
            // Convert CONST values to decimals
            if( line.find("CONST") != string::npos ) line = transformDec(line, hexRegex);

            if(!line.empty()) outFile << line << nl;
        }
    }
}
