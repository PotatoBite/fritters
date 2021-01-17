#include <iostream>

#include <ciphers.cpp>

using namespace std;

int main(){
    
    int result = 0;
    string test = "estaba la pajara pinta\nsentada en el verde limon";

    //caesar
    if( caesar(caesar(test, 34), -34) != test) {
        cout<< "caesar failed";
        result = -1;
    } 
    //vigenere
    if( vigenere(vigenere(test, "mambo#5"), "mambo#5", true) != test) {
        cout<< "vigenere failed";
        result = -1;
    } 

    return result;
}