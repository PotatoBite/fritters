//All implementation treats words as uint8_t

#include <stdint.h>
#include <math.h>

//constant-distance rotation
#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))

//union for use in little endian systems only, and avoid conversion on salsa20 function
union little_endiands_union_word_and_4bytes
{
    uint8_t bytes_array [4];
    uint32_t word;
};

/**
 * Union little_endiands_union_word_and_4bytes, representing
 * an array of 4 bytes(uint8_t), and a word(uint32_t)
 **/
typedef little_endiands_union_word_and_4bytes LEUW4B;

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



//row-round of salsa20, not in use
/**
 * @returns pointer to `uint32_t[16]`
**/
inline uint32_t* row_round(uint32_t input[16]){
	
	/**C++ does not advocate to return the address of a local variable to outside of the function so 
	 * you would have to define the local variable as static variable.
	 **/
	static uint32_t output[16];



	return output;
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

//not working
void inplace_salsa20(uint8_t input[64]){
	uint32_t x[16];
	uint32_t z[16];

	uint8_t temp_input[4];
	for(int i = 0 ; i < 16 ; i++){
		temp_input[0] = input[i * 4 + 0];
		temp_input[1] = input[i * 4 + 1];
		temp_input[2] = input[i * 4 + 2];
		temp_input[3] = input[i * 4 + 3];

		x[i] = littleendian(temp_input);
	}

	for(int i = 0 ; i < 16 ; i++){
		z[i] = x[i];
	}

	for (int i = 0 ; i < 10 ; i++ ){
		double_round_in_place(z);
	}
	
}