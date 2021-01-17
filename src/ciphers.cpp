#include <iostream>
#include <fstream>
#include <sstream>

#include "fritters/caesar.cpp"
#include "fritters/vigenere.cpp"

//utils
std::string slurp(std::ifstream& in) {
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}