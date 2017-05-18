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

void BitOutputStream::writeByte(int integer){
	//out.put(i);
	int bit;
	for(int i = 0;i < 8;i++){
		bit = 1 & (integer >> (7-i));
		//cout<<bit<<endl;
		writeBit(bit);
	}
}

void BitOutputStream::writeInt(int integer){
	int bit;
	for(int i = 0;i < 32;i++){
		bit = 1 & (integer >> (31-i));
		//cout<<bit<<endl;
		writeBit(bit);
	}
}