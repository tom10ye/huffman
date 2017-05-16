#include "BitOutputStream.h"

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

void BitOutputStream::writeByte(int i){
	// for(int iter=0; iter<8;iter++){
	// 	out->writeBit((i>>(7-iter)) & 1);
	// }
	out.put(i);
}