#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>

// TODO implement characters for bases higher than 62 ?

std::vector<char> validNumerals = {'0','1','2','3','4','5','6','7','8','9'};
std::vector<char> extendedValidNumerals = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u'
,'v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
std::map<char, int> numMap{{'0',0},{'1',1},{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9},{'a',10},{'b',11},{'c',12},{'d',13},
{'e',14},{'f',15},{'g',16},{'h',17},{'i',18},{'j',19},{'k',20},{'l',21},{'m',22},{'n',23},{'o',24},{'p',25},{'q',26},{'r',27},{'s',28},{'t',29},{'u',30},{'v',31},{'w',32},
{'x',33},{'y',34},{'z',35},{'A',36},{'B',37},{'C',38},{'D',39},{'E',40},{'F',41},{'G',42},{'H',43},{'I',44},{'J',45},{'K',46},{'L',47},{'M',48},{'N',49},{'O',50},{'P',51},
{'Q',52},{'R',53},{'S',54},{'T',55},{'U',56},{'V',57},{'W',58},{'X',59},{'Y',60},{'Z',61}};

bool validBase(std::string& base) {
    if (base == "" | base == "0" | base == "1" | base == "-1" | base == "-") return false;

    if (base[0] != '-' & std::find(validNumerals.begin(), validNumerals.end(), base[0]) == validNumerals.end()) return false;

    for (uint i=1; i<base.size(); i++) {
        if (std::find(validNumerals.begin(), validNumerals.end(), base[i]) == validNumerals.end()) return false;
    }

    return true;
}

bool validNum(int& base, std::string& val) {
    if (val == "" | val == "-") return false;

    uint i=0;
    if (val[0] == '-') i = 1;

    while (i<val.size()) {
        if (std::find(extendedValidNumerals.begin(), extendedValidNumerals.end(), val[i]) == extendedValidNumerals.end()) return false;
        if (numMap[val[i]] >= std::abs(base)) return false;
        i++;
    }

    return true;
}

int toDec(int& base, std::string& val) {
    int res = 0;
    int mult = 1;

    bool negative = false;
    if (val[0] == '-') {
        negative = true;
    }

    for (uint i=0; i<val.size()- negative ? 1:0; i++) {
        res += mult * numMap[val[val.size()-i-1]];
        mult *= base;
    }

    return res * (negative ? -1 : 1);
}

std::string toBase(int& base, int val) {
    if (val == 0) return "0";
    
    std::string res = "";

    bool negative = false;
    if (val < 0 & base > 0) {
        val *= -1;
        negative = true;
    }

    for (;;) {
        int mult = val / base;
        int remainder = val - mult*base;
        while (remainder < 0) {
            mult++;
            remainder = val - mult*base;
        }
        res += std::to_string(remainder);
        if (mult == 0) break;
        val = mult;
    }
    
    if (negative) res += '-';
    std::reverse(res.begin(), res.end());
    return res;
}

int main() {
    std::string val;
    std::string baseFrom;
    std::string baseTo;
    int iBaseFrom;
    int iBaseTo;

    std::cout << "Lowercase letters go before uppercase, so a=10 and A=36. Bases are in decimal only" << std::endl << std::endl;

    std::cout << "Input value: ";
    std::cin >> val;
    std::cout << "Input base value is in: ";
    std::cin >> baseFrom;
    std::cout << "Input base to convert to: ";
    std::cin >> baseTo;

    while (!validBase(baseFrom)) {
        std::cout << "Invalid base " << baseFrom << std::endl;
        std::cout << "Input base value is in: ";
        std::cin >> baseFrom;
    }

    while (!validBase(baseTo)) {
        std::cout << "Invalid base " << baseTo << std::endl;
        std::cout << "Input base to convert to: ";
        std::cin >> baseTo;
    }

    iBaseFrom = std::stoi(baseFrom);
    iBaseTo = std::stoi(baseTo);

    while (!validNum(iBaseFrom, val)) {
        std::cout << "Invalid value " << val << " for base " << baseFrom << std::endl;
        std::cout << "Input value: ";
        std::cin >> val;
    }

    std::cout << val << " in base " << baseFrom << " converted to base " << baseTo << " is " << toBase(iBaseTo, toDec(iBaseFrom, val)) << std::endl;

    return 0;
}