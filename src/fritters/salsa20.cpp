//All implementation treats words as uint32_t or uint8_t[4]
#include <iostream>
#include <array>
#include <iostream>
#include <stdint.h>
#include <math.h>
//#include <bits/stdc++.h>
#include "xor.h"

//constant-distance rotation
#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))

//union for use in little endian systems only, and avoid conversion on salsa20 function
union little_endiands_union_word_and_4bytes
{
    uint8_t bytes_array [4];
    uint32_t word;
};

union little_endiands_union_2word_and_8bytes
{
	uint64_t word;
    uint8_t bytes_array [8];
};

/**
 * Union `little_endiands_union_word_and_4bytes`, representing
 * an array of 4 bytes(uint8_t), or a word(uint32_t)
 **/
typedef little_endiands_union_word_and_4bytes LEUW4B;

/**
 * Union `little_endiands_union_2word_and_8bytes`, representing
 * an array of 8 bytes(uint8_t), or two words(uint64_t)
 **/
typedef little_endiands_union_2word_and_8bytes LEU2W8B;


//quarter-round core of salsa20
/**
 * One can visualize the quarterround function as modifying `yi` in place. 
 * Each modification is invertible, so the entire function is invertible.
**/
inline void quarter_round(uint32_t &y0, uint32_t &y1, uint32_t &y2, uint32_t &y3){
	y1 = y1 ^ ROTL(y0 + y3, 7);
	y2 = y2 ^ ROTL(y1 + y0, 9);
	y3 = y3 ^ ROTL(y2 + y1, 13);
	y0 = y0 ^ ROTL(y3 + y2, 18);
}




//row-round of salsa20
/**
 * The `row_round_in_place` function modifies the rows of the input 4x4 matrix(`uint32_t input[16]`) in parallel by feeding a
 * permutation of each row through the `quarter_round` function.
**/
inline void row_round_in_place(uint32_t input[16]){
	quarter_round(input[0],input[1],input[2],input[3]);
	quarter_round(input[5],input[6],input[7],input[4]);
	quarter_round(input[10],input[11],input[8],input[9]);
	quarter_round(input[15],input[12],input[13],input[14]);
	/*
	for (int i = 0, offset = 0; i < 16 ; i += 4, offset++){
		quarter_round(input[(i + offset) % ((offset + 1) * 4)],input[(i + 1 + offset) % ((offset + 1) * 4)],input[(i + 2 + offset) % ((offset + 1) * 4)],input[(i + 3 + offset) % ((offset + 1) * 4)]);
	}
	*/
}



//column-round of salsa20
/**
 * The `column_round_in_place` function is, simply the transpose of the
 * `row_round_in_place` function: it modifies the columns of the input 4x4 matrix(`uint32_t input[16]`) in parallel 
 * by feeding a permutation of each column through the `quarter_round` function.
**/
inline void column_round_in_place(uint32_t input[16]){
	quarter_round(input[0],input[4],input[8],input[12]);
	quarter_round(input[5],input[9],input[13],input[1]);
	quarter_round(input[10],input[14],input[2],input[6]);
	quarter_round(input[15],input[3],input[7],input[11]);
}



//double-round of salsa20
/**
 * One can visualize a `double_round_in_place` as modifying the columns of the input 4x4 matrix(`uint32_t input[16]`) in
 * parallel, and then modifying the rows in parallel. Each word(`uint32_t`) is modified twice.
**/
inline void double_round_in_place(uint32_t input[16]){
	column_round_in_place(input);
	row_round_in_place(input);
}


/**The input is set to uint8_t, becouse in the specs is defined like:
 * If input is a 4-byte sequence then littleendian(b) is a word.
 * And all implementation treats words as uint8_t.
**/
inline uint32_t littleendian(uint8_t input[4]){
	return  input[0] + (pow(2, 8) * input[1]) + (pow(2, 16) * input[2]) + (pow(2, 24) * input[3]); 
}




//just for little endian systems for now, need to implement an inverse of the littleendian function above
/**
 * Salsa20 hash function.
 * 
 * this does all operations in place, in @param input. 
**/
void inplace_salsa20(LEUW4B input[16]){
	uint32_t z[16];
	
	for(int i = 0 ; i < 16 ; i++){
		z[i] = input[i].word;
	}

	for (int i = 0 ; i < 10 ; i++ ){
		double_round_in_place(z);
	}

	for(int i = 0 ; i < 16 ; i++){
		input[i].word += z[i];
	}

}

/**
 * Salsa20 hash function.
 * 
 * this does all operations in place, in @param input. 
**/
void inplace_salsa20(std::array<LEUW4B,16>& input){
	uint32_t z[16];
	
	for(int i = 0 ; i < 16 ; i++){
		z[i] = input[i].word;
	}

	for (int i = 0 ; i < 10 ; i++ ){
		double_round_in_place(z);
	}

	for(int i = 0 ; i < 16 ; i++){
		input[i].word += z[i];
	}

}

//constants for calculation,  sigma=“expand 32-byte k”, tau=“expand 16-byte k”
static const LEUW4B sigma[4] = {101, 120, 112, 97,
								110, 100, 32, 51,
								50, 45, 98, 121,
								116, 101, 32, 107
								};

static const LEUW4B tau[4] = {	101, 120, 112, 97,
								110, 100, 32, 49,
								54, 45, 98, 121,
								116, 101, 32, 107
							 };

/**
 * Salsa20 expansion function
 * @param key must be of size 4 LEUW4B(128 bits) or 8 LEUW4B(256 bits).
 * @returns array of `16 LEUW4B`
 * @param key_bits tells the size of key, if `256`, uses whole key, else uses only half, also this changes some constants
 * 
 * “Expansion” refers to the expansion of (key, nonce) into Salsa20k(n). It also refers to
 * the expansion of k into a long stream of Salsa20k outputs for various n’s.
 * (see oficial specs `Salsa20 specification` by `Daniel J. Bernstein`);
 **/
std::array<LEUW4B,16> salsa20_expansion(LEUW4B key[8], LEUW4B nonce[4], uint32_t key_bits){
	
	/**C++ does not advocate to return the address of a local variable to outside of the function so 
	 * you would have to define the local variable as static variable.
	 **/
	//static LEUW4B output[16];

	std::array<LEUW4B,16> output;

	static const LEUW4B* constant;
	if (key_bits == 256 ){
		constant = sigma;
	}
	else{//key_bits == 128
		constant = tau;
	} 

	output[0].word = constant[0].word;
	for (size_t i = 0; i < 4; i++){
		output[1 + i].word = key[i].word;
	}
	output[5].word = constant[1].word;
	for (size_t i = 0; i < 4; i++){
		output[6 + i].word = nonce[i].word;
	}
	output[10].word = constant[2].word;
	if (key_bits == 256 ){
		for (size_t i = 0; i < 4; i++){
			output[11 + i].word = key[4 + i].word;
		}
	}
	else{//key_bits == 128
		for (size_t i = 0; i < 4; i++){
			output[11 + i].word = key[i].word;
		}
	} 
	output[15].word = constant[3].word;
	
	inplace_salsa20(output);

	return output;

}

/**
 * salsa20 encrytion function, over the @param message
 * @param nonce is 8-byte secuence(2 words, 2 LEUW4B, 8 uint8_t, 2 uint32_t), for combining with block index before 
 * calculation with `salsa20_expansion`.
 * @param key need to be 32(recommended) or 16 bytes secuence.
 * */
void inplace_salsa20_encryption(std::string &message,const std::string &key, const std::string &nonce){
	
	if(nonce.size() > 8 || key.size() > 32){
		//not good params
	}
	
	
	LEUW4B key_to_expand[8];
	//copy key to LEUW4B key_to_expand, char by char, repeating string if not enough 
	for (size_t i = 0; i < 32; i++){	
		key_to_expand[ int( floor(i / 4.0) ) ].bytes_array[i % 4] = key[i % key.length()];
	}
	
	for (size_t i = 0; i < 32; i++){
		std::cout <<key_to_expand[int( floor(i / 4.0))].bytes_array[i % 4];
	}
	std::cout << std::endl;


	LEUW4B half_nonce[2];
	//copy nonce to LEUW4B half_nonce, char by char, repeating string if not enough 
	for (size_t i = 0; i < 8; i++){
		half_nonce[ int( floor(i / 4.0 ) ) ].bytes_array[i % 4] = nonce[i % nonce.length()];
	}
	for (size_t i = 0; i < 8; i++){
		std::cout << half_nonce[ int( floor(i / 4.0 ) ) ].bytes_array[i % 4];
	}
	std::cout << std::endl;

	std::array<LEUW4B,16> block;
	static const uint64_t blocks_count = ceil(message.size() / 64.0);

	//nonce to be passed to salsa20_expansion
	LEUW4B temp_full_nonce[4];
	temp_full_nonce[0] = half_nonce[0];
	temp_full_nonce[1] = half_nonce[1];

	for (LEU2W8B i = { 0 }; i.word < blocks_count; i.word++) {
		
		//complete nonce with the other half(block of data index)
		//temp_full_nonce = half_nonce + 
		for (size_t ii = 0; ii < 2; ii++)
		{
			for (size_t iii = 0; iii < 4; iii++)
			{
				temp_full_nonce[2 + ii].bytes_array[iii] = i.bytes_array[ii * 4 + iii];
			}
			
		}
		//passed 256 cause the key_to_expand is constructed repeating the key multiple times til key_to_expand is apropiate sized
		auto block_cipher = salsa20_expansion(key_to_expand, temp_full_nonce, 256);
		for (size_t ii = 0; ii < 64; ii++){
			if ((i.word + ii) < message.size()){
				message[i.word + ii] ^= block_cipher[ floor( ii / double(4) )].bytes_array[ ii % 4];
			}
			else break;
		}
		
	}
	

}
