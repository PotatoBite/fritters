#include "utils.h"

//utils
//read a file(ifstream) to string
std::string slurp(std::ifstream& in) 
{
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

//this does not work
/*std::string asciistring2hexstring(const std::string &ascii)//WARNING: this function if not introduced a correct string could break execution
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
*/


std::string string2hexstring(const std::string &str)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < str.size(); ++i)
    {
        ss << std::setw(2) << static_cast<unsigned>(str[i]);
    }
    return ss.str();
}

std::string hexstring2asciistring(const std::string &hex)
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