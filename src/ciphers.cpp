#include <iostream>
#include <fstream>
#include <sstream>

#include "fritters/caesar.cpp"
#include "fritters/vigenere.cpp"
#include "fritters/xor.cpp"

//utils
//read a file(ifstream) to string
std::string slurp(std::ifstream& in) {
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}