#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

std::string slurp(std::ifstream& in); 


std::string string2hexstring(const std::string &str);//introduces weird chars

/**
 * @brief Converts from hex string to ASCII string
 * 
 * @param hex a string represented by hex numbers(in pairs ie: BB)
 * @return std::string a string coded in ASCII
 */
std::string hexstring2asciistring(const std::string &hex);//WARNING: this function if not introduced a correct string could break execution