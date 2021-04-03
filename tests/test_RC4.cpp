#include <iostream>
#include <fritters/RC4.cpp>
#include <sstream>
#include <iomanip>
#include <vector>

struct test_RC4_vector
{
    std::string key;
    std::string plaintext;
    std::string ciphertext;
};

bool test_RC4();
std::string string2hexstring(const std::string &str);

/**
 * @brief Converts from hex string to ASCII string
 * 
 * @param hex a string represented by hex numbers(in pairs ie: BB)
 * @return std::string a string coded in ASCII
 */
std::string hexstring2asciistring(const std::string &hex);

int main()
{
    int result = 0;

    //testing quarter round by set of input/output
    if(!test_RC4()) result = -1;

    return result;
}

bool test_RC4(){
    int result = 0;
    std::vector<test_RC4_vector> input_words_array = { {"Key", "Plaintext", "BBF316E8D940AF0AD3"},
                                                    {"Wiki", "pedia", "1021BF0420"},
                                                    {"Secret", "Attack at daw", "45a01f645fc35b383552544b9b"}, 
                                                    {"Secret", "Attack at dawn", "45a01f645fc35b383552544b9bf5"},
                                                    {"Secret", "Attack at dawno", "45a01f645fc35b383552544b9bf5d6"}, 
                                                    {"Secret", "Attack at dawnaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "45a01f645fc35b383552544b9bf5d8f35370b554382e61167ca8783e291791e69214251d774c534fa48ccb70f7d73edba6be"},
                                                    {"mariconchi.com", "Attack at dawnaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "522708d05d27a2d882b0f02a37af454be28a0f84d5ac113edd7d77a3f00f8699520ac9a4f959e2e2c9e1c0d4ca0f55d1aa9c"}
    };

    for (int i=0; i<input_words_array.size(); i++)
    {
        RC4 cipher(input_words_array[i].key);
        cipher.inplaceCipher(input_words_array[i].plaintext);   
    }

    for (int i=0; i<input_words_array.size(); i++){
        if (input_words_array[i].plaintext != hexstring2asciistring(input_words_array[i].ciphertext))
        {
            result = -1;
            std::cout << "\trow round failed on input #: "<< i + 1 << std::endl <<"\t";
            std::cout << string2hexstring(input_words_array[i].plaintext)<< std::endl <<"\t";
            std::cout << input_words_array[i].ciphertext << std::endl;  
        }
    }

    if (result == 0) return true; 
    
    return false;
}

std::string string2hexstring(const std::string &str)//introduces weird chars
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < str.size(); ++i)
    {
        ss << std::setw(2) << static_cast<unsigned>(str[i]);
    }
    return ss.str();
}

std::string hexstring2asciistring(const std::string &hex)//WARNING: this function if not introduced a correct string could break execution
{
   std::string ascii = "";
   for (size_t i = 0; i < hex.length(); i += 2){
      //taking two characters from hex string
      std::string part = hex.substr(i, 2);
      //changing it into base 16
      char ch = stoul(part, nullptr, 16);
      //putting it into the ASCII string
      ascii += ch;
   }
   return ascii;
}


std::string asciistring2hexstring(const std::string &ascii)//WARNING: this function if not introduced a correct string could break execution
{
   std::string hex = "";
   for (size_t i = 0; i < ascii.length(); i++){
      //taking two characters from hex string
      std::string part = ascii.substr(i, 2);
      //changing it into base 16
      char ch = stoul(part, nullptr, 16);
      //putting it into the hex string
      hex += ch;
   }
   return hex;
}