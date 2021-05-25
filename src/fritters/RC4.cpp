#include"RC4.h"
#include<iostream>

RC4::RC4(const std::string &key)
{
    KSA(key);
}

RC4::~RC4()
{
}


void RC4::KSA(std::string key){
    //restart index of keystream variables, in case of regenerating state array to avoid wrong use of cipher
    Ki = 0, Kj = 0;

    //short keylength =  key.length();
    short keylength =  key.size();

    //initialize state array to identity permutation
    for (short i = 0; i < 256; i++)
    {
        S[i] = i;
    }

    short j = 0;
    for (short i = 0; i < 256; i++)
    {
        //j = (j + S[i] + key[i % keylength]) % 256;
        j = (j + S[i] + static_cast<uint8_t>(key[i % keylength])) % 256;

        //swap S[i and S[j]]
        uint8_t tempValue = S[i];
        S[i] = S[j];
        S[j] = tempValue;
    }
}

uint8_t RC4::getKeystreamValue()
{
    //std::cout<< std::dec << Ki <<"-";
    Ki = (Ki + 1) % 256;
    Kj = (Kj + S[Ki] ) % 256;

    //swap S[i and S[j]]
    uint8_t temp = S[Ki];
    S[Ki] = S[Kj];
    S[Kj] = temp;

    uint8_t KS = S[(S[Ki] + S[Kj]) % 256];
    return KS;
}

void RC4::inplaceCipher(std::string &data)
{
    for (size_t i = 0; i <  data.size(); i++)
    {
        uint8_t a = getKeystreamValue();
        //std::cout<< std::hex << (int)a << " ";
        data[i] ^= a;
    }
}

void RC4::getStateArray(uint8_t S[256])
{
    for (short i = 0; i < 256; i++)
    {
        S[i] = (*this).S[i];
    }
}