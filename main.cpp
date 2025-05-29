//////////////////////////////////////////////////////
// Name: Base Converter                             //
//                                                  //
// Author: Alistair Journeaux                       //
// Contact: ajemail.main@gmail.com                  //
//                                                  //
// License: GNU General Public License (GPL) v3     //
// (https://www.gnu.org/licenses/gpl-3.0.en.html)   //
//                                                  //
// Copyright (C) 2025 Alistair Journeaux            //
//////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>

// TODO implement characters for bases higher than 62 ?

std::vector<char> validNumerals = {'0','1','2','3','4','5','6','7','8','9'};
std::vector<char> extendedValidNumerals = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u'
,'v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
std::map<char, int> numMap{{'0',0},{'1',1},{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9},{'a',10},{'b',11},{'c',12},{'d',13},
{'e',14},{'f',15},{'g',16},{'h',17},{'i',18},{'j',19},{'k',20},{'l',21},{'m',22},{'n',23},{'o',24},{'p',25},{'q',26},{'r',27},{'s',28},{'t',29},{'u',30},{'v',31},{'w',32},
{'x',33},{'y',34},{'z',35},{'A',36},{'B',37},{'C',38},{'D',39},{'E',40},{'F',41},{'G',42},{'H',43},{'I',44},{'J',45},{'K',46},{'L',47},{'M',48},{'N',49},{'O',50},{'P',51},
{'Q',52},{'R',53},{'S',54},{'T',55},{'U',56},{'V',57},{'W',58},{'X',59},{'Y',60},{'Z',61}};

// Checks characters in string to see if base is valid. 
bool validBase(std::string& base) {
    if (base == "" || base == "0" || base == "1" || base == "-1" || base == "-") return false;

    if (base[0] != '-' && std::find(validNumerals.begin(), validNumerals.end(), base[0]) == validNumerals.end()) return false;

    for (unsigned int i=1; i<base.size(); i++) {
        if (std::find(validNumerals.begin(), validNumerals.end(), base[i]) == validNumerals.end()) return false;
    }

    return true;
}

// Checks characters in string with a given base to see if integer is valid.
bool validNum(int& base, std::string& val) {
    if (val == "" || val == "-") return false;

    unsigned int i=0;
    if (val[0] == '-') i = 1;

    while (i<val.size()) {
        if (std::find(extendedValidNumerals.begin(), extendedValidNumerals.end(), val[i]) == extendedValidNumerals.end()) return false;
        if (numMap[val[i]] >= std::abs(base)) return false;
        i++;
    }

    return true;
}

// Converts string to a decimal integer. String must be validated first.
int toDec(int& base, std::string& val) {
    int res = 0;
    int mult = 1;

    bool negative = false;
    if (val[0] == '-') {
        negative = true;
    }

    for (unsigned int i=0; i<val.size()-negative ? 1:0; i++) {
        res += mult * numMap[val[val.size()-i-1]];
        mult *= base;
    }

    return res * (negative ? -1 : 1);
}

// Converts decimal integer val into given base system.
std::string toBase(const int& base, int val) {
    if (val == 0) return "0";
    
    std::string res = "";

    bool negative = false;
    if (val < 0 && base > 0) {
        val *= -1;
        negative = true;
    }

    int mult = 0;
    do  {
        mult = val / base;
        int remainder = val - mult*base;
        while (remainder < 0) {
            mult++;
            remainder = val - mult*base;
        }
        res += std::to_string(remainder);
        if (mult != 0){
            val = mult;
        }
    } while (mult != 0);
    
    if (negative) res += '-';
    std::reverse(res.begin(), res.end());
    return res;
}

// Splits input string by delimeter and places into arr.
void splitByDelim(const std::string& inp, std::string& temp, std::vector<std::string>& arr, const char delim = ',') {
    std::stringstream ssVals(inp);
    while (std::getline(ssVals, temp, delim)) {
        // only push if element is not in arr
        if (std::find(arr.begin(), arr.end(), temp) == arr.end()) arr.push_back(temp);
    }
}

int main(int argc, char *argv[]) {
    for (;;) {
        std::string inp;
        std::vector<std::string> vals;
        std::vector<std::string> invalidVals;
        std::vector<std::string> validVals;
        std::string baseFrom;
        std::vector<std::string> basesTo;
        std::vector<std::string> invalidBasesTo;
        std::vector<std::string> validBasesTo;
        int iBaseFrom;
        std::vector<int> iBasesTo;
        std::string temp;

        // If there is a console argument then use inputs from a file
        if (argc > 1) {

            std::ifstream file;
            std::vector<std::string> args;

            // Push file into vector
            file.open(argv[1]);
            if (!file.is_open()) { std::cout << "Unable to open file." << std::endl; return 0; }
            while(getline(file, temp, '\n')) {
                args.push_back(temp);
            }
            file.close();

            // Must be 3 args
            if (args.size() < 3) { std::cout << "Incorrect file layout." << std::endl; return 0;}

            // Validate value's base
            if (!validBase(args[0])) { std::cout << "Invalid base: " << args[0] << std::endl; return 0; }
            baseFrom = args[0];

            // Split values
            splitByDelim(args[1], temp, vals);

            // Split bases to convert to
            splitByDelim(args[2], temp, basesTo);

        } else {

            std::cout << "\033[2J\033[1;1H";    // Clears the screen

            // Get base values are in
            std::cout << "Input base, in decimal form, integers are in. Negative bases begin with '-': ";
            std::cin >> baseFrom;

            // Validate value's base
            while (!validBase(baseFrom)) {
                std::cout << "Invalid base " << baseFrom << std::endl;
                std::cout << "Input base value is in: ";
                std::cin >> baseFrom;
            }
            std::cout << std::endl;

            // Get values
            std::cout << "Input integers seperated by commas, negative integers begin with '-'." << std::endl;
            std::cout << "Valid characters are 0-9, a-z, A-Z; where a=10 and A=36." << std::endl;
            std::cout << "Note that characters must be valid for the base they are in. Do not input whitespace." << std::endl;
            std::cin >> inp;
            splitByDelim(inp, temp, vals);

            std::cout << std::endl;

            // Get bases to convert to
            std::cout << "Input bases to convert to in decimal form, seperated by commas. Negative bases begin with '-'." << std::endl;
            std::cin >> inp;
            splitByDelim(inp, temp, basesTo);

        }

        // Validate conversion bases
        for (std::string b : basesTo) {
            if (!validBase(b)) invalidBasesTo.push_back(b);
            else validBasesTo.push_back(b);
        }

        // Get bases as ints
        iBaseFrom = std::stoi(baseFrom);
        for (std::string e : validBasesTo) iBasesTo.push_back(std::stoi(e));

        // Validate values
        for (std::string v : vals) {
            if (!validNum(iBaseFrom, v)) { invalidVals.push_back(v); }
            else { validVals.push_back(v); }
        }

        // Output to file if user asked, else print to console
        std::string mode;
        if (argc > 2) mode.assign(argv[2]);
        if (mode == "output") {

            std::ofstream file;
            file.open("output.txt");

            if (invalidVals.size() > 0) {
                file << "These values are invalid: ";
                for (std::string e : invalidVals) {
                    file << e << " ";
                }
                file << std::endl;
            }
            if (invalidBasesTo.size() > 0) {
                file << "These bases are invalid: ";
                for (std::string e : invalidBasesTo) {
                    file << e << " ";
                }
                file << std::endl;
            }

            file << std::endl;

            for (std::string v : validVals) {
                for (int b : iBasesTo) {
                    file << v << " in base " << baseFrom << " converted to base " << b << " is " << toBase(b, toDec(iBaseFrom, v)) << std::endl;
                }
                file << std::endl;
            }

            file.close();
            return 0;

        } else {

            std::cout << "\033[2J\033[1;1H";    // Clears the screen

            // Print invalid integers and bases
            if (invalidVals.size() > 0) {
                std::cout << "These values are invalid: ";
                for (std::string e : invalidVals) {
                    std::cout << e << " ";
                }
                std::cout << std::endl;
            }
            if (invalidBasesTo.size() > 0) {
                std::cout << "These bases are invalid: ";
                for (std::string e : invalidBasesTo) {
                    std::cout << e << " ";
                }
                std::cout << std::endl;
            }

            std::cout << std::endl;

            // Print converted integers
            for (std::string v : validVals) {
                for (int b : iBasesTo) {
                    std::cout << v << " in base " << baseFrom << " converted to base " << b << " is " << toBase(b, toDec(iBaseFrom, v)) << std::endl;
                }
                std::cout << std::endl;
            }

        }
        
        std::cout << std::endl;

        // User can reinput without rerunning
        while (inp != "start" && inp != "quit") {
            std::cout << "Input Start to begin or Quit to exit: ";
            std::cin >> inp;
            std::transform(inp.begin(), inp.end(), inp.begin(),
              [](unsigned char c){ return std::tolower(c); });
        }

        std::cout << "\033[2J\033[1;1H";    // Clears the screen
        if (inp == "quit") return 0;
    }

    return 0;
}