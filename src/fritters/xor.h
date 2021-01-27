#pragma once
#include <iostream>
#include <fstream>

std::string xor_encrypt(const std::string &str_unprocessed,const std::string &key);

inline void inplace_xor_encrypt(std::string &str,const std::string &key);
