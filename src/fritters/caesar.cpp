#include "caesar.h"

std::string caesar(const std::string &str_unprocessed,const short key){
   std::string str_processed;
   str_processed.reserve(str_unprocessed.size());

   for (std::string::size_type i = 0; i < str_unprocessed.size(); i++) {
      str_processed += str_unprocessed[i] + key;
   }
   return str_processed;
}