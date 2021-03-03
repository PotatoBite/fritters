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
        RC4(std::string key);

        ~RC4();//class destructor

        /**
         * @brief Cipher/decipher
         * 
         * @param data Plain or ciphered text 
         */
        void inplaceCipher(std::string &data);
};


RC4::RC4(std::string key)
{
    KSA(key);
    for (size_t i = 0; i < 256; i++)
    {
        std::cout<< S[i];
    }
    std::cout<< std::endl;
}

RC4::~RC4()
{
}


void RC4::KSA(std::string key){

    //restart index of keystream variables, in case of regenerating state array to avoid wrong use of cipher
    Ki = 0, Kj = 0;

    uint8_t keylength =  key.size();

    //initialize state array to identity permutation
    for (uint8_t i = 0; i < 255; i++)
    {
        S[i] = i;
    }
    
    uint8_t j = 0;
    uint8_t tempValue = 0;
    for (uint8_t i = 0; i < 255; i++)
    {
        j = (j + S[i] + key[i % keylength]) % 256;

        //swap S[i and S[j]]
        tempValue = S[i];
        S[i] = S[j];
        S[j] = S[i];
    }
}

uint8_t RC4::getKeystreamValue()
{
    Ki = (Ki + 1) % 256;
    Kj = (Kj + S[Ki] ) % 256;

    //swap S[i and S[j]]
    uint8_t temp = S[Ki];
    S[Ki] = S[Kj];
    S[Kj] = S[Ki];

    return S[(S[Ki] + S[Kj]) % 256];
}

void RC4::inplaceCipher(std::string &data)
{
    for (size_t i = 0; i <  data.size(); i++)
    {
        data[i] ^= getKeystreamValue();
    }
}