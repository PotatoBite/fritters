//to check
unsigned char S[256];
unsigned int i, j;

void swap(unsigned char *s, unsigned int i, unsigned int j) {
    unsigned char temp = s[i];
    s[i] = s[j];
    s[j] = temp;
}

/* KSA - key scheduling algorithm*/
void rc4_init(unsigned char *key, unsigned int key_length) {
    for (i = 0; i < 256; i++)
        S[i] = i;
 
    for (i = j = 0; i < 256; i++) {
        j = (j + key[i % key_length] + S[i]) & 255;
        swap(S, i, j);
    }
 
    i = j = 0;
}
 
/* PRGA - pseudo random generation algorithm*/
unsigned char rc4_output() {
    i = (i + 1) & 255;
    j = (j + S[i]) & 255;

    swap(S, i, j);

    return S[(S[i] + S[j]) & 255];
}
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

int main() {
    unsigned char *test_vectors[][2] = 
    {
        {"Key", "Plaintext"},
        {"Wiki", "pedia"},
        {"Secret", "Attack at dawn"}
    };
 
    int x;
    for (x = 0; x < ARRAY_SIZE(test_vectors); x++) {
        int y;
        rc4_init(test_vectors[x][0], strlen((char*)test_vectors[x][0]));

        for (y = 0; y < strlen((char*)test_vectors[x][1]); y++)
            printf("%02X", test_vectors[x][1][y] ^ rc4_output());
        printf("\n");
    }
    getch(); 
    return 0;
}