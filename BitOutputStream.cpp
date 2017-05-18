#include "BitOutputStream.h"
using namespace std;

void BitOutputStream::flush(){
	out.put(buf);
	out.flush();
	buf = nbits = 0;
}

void BitOutputStream::writeBit(int i){
	// Is the bit buffer full? Then flush it.		
	if(nbits == 8) flush();																		


	// Write the least significant bit of i into the buffer
	// at the current index
	buf = buf | (i<<(7-nbits));


	// Increment the index
	nbits = nbits+1;
}

//used to write a byte into the file, 1byte = 8 bits
//right shift the integer and logically "and" with 1 to get the bit we need to write
//finally call the writbit function(which has defined in the same class as above) to write the bit into buffer
//we do not need to consider the subsequent operations because we have well defined the write bit function
void BitOutputStream::writeByte(int integer){
	//out.put(i);
	int bit;
	for(int i = 0;i < 8;i++){
		bit = 1 & (integer >> (7-i));
		writeBit(bit);
	}
}


//used to write an Integer into the file, 1 int takdes 4byte = 32 bits
void BitOutputStream::writeInt(int integer){
	int bit;
	for(int i = 0;i < 32;i++){
		bit = 1 & (integer >> (31-i));
		writeBit(bit);
	}
}