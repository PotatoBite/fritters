#pragma once

#include<iostream>

/**
 * @brief RC4 cipher instance, to be populated with a 32 bytes secuence
 * 
 */
class RC4
{
    private:
        uint8_t S[256];//state array
        //size_t Ki = 0, Kj = 0;//i think that is not needed to be size_t, but not really sure, cause in some point was set to this can not remember why 
        uint8_t Ki = 0, Kj = 0;//indexes to get keystream from, Ki mark the number of bytes previously ciphered 

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
         * @brief Key-scheduling algorithm. Initialize the permutation in the state array (S)
         * 
         * @param key 
         */
        void KSA(const std::string key);

        /**
         * @brief Key-scheduling algorithm. Initialize the permutation in the state array (S)
         * and fills `jArray` with the resulting `j` index in each iteration of KSA
         * 
         * @param key 
         * @param jArray 256 length array of uint_8, to fill.
         */
        void KSA(const std::string key, uint8_t jArray[256]);


        /**
         * @brief Cipher/decipher
         * 
         * @param data Plain or ciphered text 
         */
        void inplaceCipher(std::string &data);


        //JUST FOR DEBUG/ANALYSIS
        /**
         * @brief Copies the currently State Array to the array passed as parameter
         * 
         * @param S array to store values of internal state array of cipher
         */
        void getStateArray(uint8_t S[256]);

        /**
         * @brief Get the Keystream Value, D E B U G object, use only if you really know what you are doing,
         *  is just a wrapper for private getKeystreamValue
         * 
         * @return uint8_t 
         */
        uint8_t getKeystreamValueDEBUG();

        ///DEBUG only to get the value of `j` index in algorithm
        uint8_t getJindex();
        ///DEBUG only to get the value of `i` index in algorithm
        uint8_t getIindex();
};