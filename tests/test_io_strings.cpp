#include <iostream>
#include <algorithm>

#include <ciphers.cpp>

using namespace std;

bool test_string_encrypt_decrypt( function<string(string)> , function<string(string)> ,const string);

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
        cout<< "\tcaesar failed" << endl;
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
        cout<< "\tvigenere failed" << endl;
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
        cout<< "\txor failed" << endl;
        result = -1;
    }

    //xor inplace
    if(!test_string_encrypt_decrypt(
        [](string unencrypted_str)->string{
            inplace_xor_encrypt(unencrypted_str, "mambo#5");
            return unencrypted_str;
        },
        [](string encrypted_str)->string{
            inplace_xor_encrypt(encrypted_str, "mambo#5" );
            return encrypted_str;
        },
        test
    )){
        cout<< "\txor inplace failed" << endl;
        result = -1;
    }

    //salsa20 inplace
    if(!test_string_encrypt_decrypt(
        [](string unencrypted_str)->string{
            inplace_salsa20_encryption(unencrypted_str, "ugsdfblikwuhri2urho28iruqrhoicnuhoiuronqidurc pi", "asdasdasdasdasdasdasdasdasd");
            return unencrypted_str;
        },
        [](string encrypted_str)->string{
            inplace_salsa20_encryption(encrypted_str, "ugsdfblikwuhri2urho28iruqrhoicnuhoiuronqidurc pi2", "asdasdasdasdasdasdasdasdasd2" );
            return encrypted_str;
        },
        test
    )){
        cout<< "\tsalsa20 inplace failed" << endl;
        result = -1;
    }

    return result;
}


/**
 * @param cipher must be a lambda function accepting a std::string as unique input, and returning a std::string
 * @param decipher must be a lambda function accepting a std::string as unique input, and returning a std::string 
 * @returns `true` if the deciphered string of the ciphered inital string, remains the same, `false` otherwise 
 * */
bool test_string_encrypt_decrypt( function<string(string)> cipher, function<string(string)> decipher,const string test_str){
    string str_encrypted = cipher(test_str);
    string str_decrypted = decipher(str_encrypted);

    if (test_str != str_encrypted && test_str == str_decrypted){
        return true;
    }
    return false;
}