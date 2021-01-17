#include "vigenere.h"

std::string vigenere(const std::string &str_unprocessed,const std::string &key,const bool reverse){
   std::string str_processed;
   str_processed.reserve(str_unprocessed.size());

   for (std::string::size_type i = 0; i < str_unprocessed.size(); i++) {
      if (reverse) 
         str_processed += str_unprocessed[i] - key[i % key.size()];
      else 
         str_processed += str_unprocessed[i] + key[i % key.size()];
   }
   return str_processed;
}