#include<iostream>

/**
 * @brief RC4 cipher instance, to be populated with a 32 bytes secuence
 * 
 */
class RC4
{
    private:
        uint8_t S[256];//state array
        size_t Ki = 0, Kj = 0;//indexes to get keystream from, Ki mark the number of bytes previously ciphered 
        
        /**
         * @brief Key-scheduling algorithm. Initialize the permutation in the state array (S)
         * 
         * @param key 
         */
        void KSA(std::string key);

        /**
         * @brief Get the Keystream Value object, depending on the global index Ki, meaning is statefull, 
         * depends on the length of the data array to be ciphered/deciphered.
         * 
         * @return uint8_t 
         */
        uint8_t getKeystreamValue();
    public:

        /**
         * @brief Construct a new RC4 object, initializing state array S
         * 
         * @param key need to be a 5-32(recommended) bytes secuence
         */
        explicit RC4(const std::string &key);

        ~RC4();//class destructor

        /**
         * @brief Cipher/decipher
         * 
         * @param data Plain or ciphered text 
         */
        void inplaceCipher(std::string &data);
};