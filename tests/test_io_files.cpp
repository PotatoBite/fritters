#include <iostream>
#include<fstream>
#include <iterator>
#include <string>
#include <algorithm>

#include <ciphers.cpp>

using namespace std;

//declarations
bool compareFiles(const std::string&, const std::string&);
bool test_file_encrypt_decrypt(function<string(string)> cipher, function<string(string)>, string);

//definitions
int main(){
    
    int result = 0;
    //string test_file_name = "test.txt";
    string test_file_name = "test.db.MH.sqlite";

    //caesar
    if(!test_file_encrypt_decrypt(
        [](string unencrypted_str)->string{
            return caesar(unencrypted_str, 3);
        },
        [](string encrypted_str)->string{
            return caesar(encrypted_str, -3);
        },
        test_file_name
    )){
        cout<< "caesar failed" << endl;
        result = -1;
    }

    //vigenere
    if(!test_file_encrypt_decrypt(
        [](string unencrypted_str)->string{
            return vigenere(unencrypted_str, "mambo#5");
        },
        [](string encrypted_str)->string{
            return vigenere(encrypted_str, "mambo#5", true);
        },
        test_file_name
    )){
        cout<< "vigenere failed" << endl;
        result = -1;
    }

    //xor
    if(!test_file_encrypt_decrypt(
        [](string unencrypted_str)->string{
            return xor_encrypt(unencrypted_str, "mambo#5");
        },
        [](string encrypted_str)->string{
            return xor_encrypt(encrypted_str, "mambo#5");
        },
        test_file_name
    )){
        cout<< "xor_encrypt failed" << endl;
        result = -1;
    }
    return result;
}


bool compareFiles(const std::string& p1, const std::string& p2) {
  std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
  std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

  if (f1.fail() || f2.fail()) {
    return false; //file problem
  }

  if (f1.tellg() != f2.tellg()) {
    return false; //size mismatch
  }

  //seek back to beginning and use std::equal to compare contents
  f1.seekg(0, std::ifstream::beg);
  f2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf()));
}

/**
 * @param cipher must be a lambda function accepting a std::string as unique input 
 * @param decipher must be a lambda function accepting a std::string as unique input 
 * @returns `true` if the deciphered file of the ciphered inital file, remains the same, `false` otherwise 
 * */
bool test_file_encrypt_decrypt(function<string(string)> cipher, function<string(string)> decipher, string path_to_file){
    
    ifstream test_file(path_to_file, ios::binary);
    if(!test_file)
    {
        cout<<"\nError Occurred, Opening the Source File (to Read)!";
        return false;
    }

    //caesar
    ofstream temp_file("tmp.txt", ios::binary);
    if(!temp_file)
    {
        cout<<"\nError Occurred, Opening/Creating the tmp File!";
        return false;
    }
    temp_file << cipher(slurp(test_file));
    temp_file.close();

    ofstream output_file("output.txt", ios::binary);
    ifstream temp_file2("tmp.txt", ios::binary);
    if(!output_file)
    {
        cout<<"\nError Occurred, Opening/Creating the tmp File!";
        return false;
    }
    output_file << decipher(slurp(temp_file2));
    output_file.close();

    if (compareFiles(path_to_file,"output.txt") && !compareFiles(path_to_file,"tmp.txt")){
        test_file.close();
        return true;
    }

    test_file.close();
    return false;
}