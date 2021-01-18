#include "xor.h"

std::string xor_encrypt(const std::string &str_unprocessed,const std::string &key){
   std::string str_processed;
   //str_processed.reserve(str_unprocessed.size());
   str_processed = str_unprocessed;

   for (std::string::size_type i = 0; i < str_unprocessed.size(); i++) {
      str_processed[i] = str_unprocessed[i] ^ key[i % key.size()];
   }
   return str_processed;
}
