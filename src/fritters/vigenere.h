#pragma once
#include <iostream>
#include <fstream>

std::string vigenere(const std::string &str_unprocessed,const std::string &key,const bool reverse = false);