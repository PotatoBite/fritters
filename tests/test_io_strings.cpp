#include <iostream>
#include <algorithm>

#include <ciphers.cpp>

using namespace std;

bool test_string_encrypt_decrypt( function<string(string)> , function<string(string)> , string);

int main(){
    
    int result = 0;
    string test = "estaba la pajara pinta\nsentada en el verde limon";

    //caesar
    if(!test_string_encrypt_decrypt(
        [](string unencrypted_str)->string{
            return caesar(unencrypted_str, 34);
        },
        [](string encrypted_str)->string{
            return caesar(encrypted_str, -34);
        },
        test
    )){
        cout<< "caesar failed" << endl;
        result = -1;
    }

    //vigenere
    if(!test_string_encrypt_decrypt(
        [](string unencrypted_str)->string{
            return vigenere(unencrypted_str, "mambo#5");
        },
        [](string encrypted_str)->string{
            return vigenere(encrypted_str, "mambo#5", true);
        },
        test
    )){
        cout<< "vigenere failed" << endl;
        result = -1;
    }

    //xor
    if(!test_string_encrypt_decrypt(
        [](string unencrypted_str)->string{
            return xor_encrypt(unencrypted_str, "mambo#5");
        },
        [](string encrypted_str)->string{
            return xor_encrypt(encrypted_str, "mambo#5" );
        },
        test
    )){
        cout<< "xor failed" << endl;
        result = -1;
    }

    return result;
}


/**
 * @param cipher must be a lambda function accepting a std::string as unique input 
 * @param decipher must be a lambda function accepting a std::string as unique input 
 * @returns `true` if the deciphered string of the ciphered inital string, remains the same, `false` otherwise 
 * */
bool test_string_encrypt_decrypt( function<string(string)> cipher, function<string(string)> decipher, string test_str){
    string str_encrypted = cipher(test_str);
    string str_decrypted = decipher(str_encrypted);

    if (test_str != str_encrypted && test_str == str_decrypted){
        return true;
    }
    return false;
}