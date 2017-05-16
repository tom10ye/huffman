#include "BitInputStream.h"


void BitInputStream::fill() {

	buf = in.get();
	nbits = 0;

}

/** Read the next bit from the bit buffer.
* Fill the buffer from the input stream first if needed.
* Return 1 if the bit read is 1;
* return 0 if the bit read is 0.
*
*/
int BitInputStream::readBit() {
// If all bits in the buffer are read, fill the buffer first
	if(nbits == 8) fill();
	
// Get the bit at the appriopriate location in the bit
// buffer, and return the appropriate int 
	int bit = ((1<<(7-nbits)) & buf)>>(7-nbits);

// Increment the index
	nbits = nbits+1;

	return bit;
}

int BitInputStream::readByte(){
	// int res = 0;
	// for(int iter=0; iter<8;iter++){
	// 	res = res | this->readBit()<<(7-iter);
	// }
	return in.get();
}