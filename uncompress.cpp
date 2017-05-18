#include "HCNode.h"
#include "HCTree.h"
#include "BitInputStream.h"
#include "BitOutputStream.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
	//1.initilize the structue and varuable we are going to use
	char* infile = argv[1];
	char* outfile = argv[2];

	ifstream in;
	ofstream out;
	//final submission add on
	BitInputStream bitIn(in);

	in.open(infile, ios::in | ios::binary);
	vector<int> freqs(256,0);

	//2.read tree header and build tree
	//Naive read  header
	/*
	for(int i = 0; i <256; i++){
		//cout<<i<<endl;
		int frequency_int = 0;
		for(int j = 0; j<32; j++){
			int bit = bitIn.readBit();
			frequency_int = frequency_int | (bit<<(31-j));
		}
		//in>>frequency_int;
		freqs[i] = frequency_int;
		//cout<<frequency_int<<endl;
	}
	*/

	//initialize tree
	HCTree tree;
	//tree.build(freqs);
	int totalNum = bitIn.readInt();
	int uniqueNum = bitIn.readByte();
	tree.readCleverHeader(bitIn, uniqueNum);

	out.open(outfile, ios::out | ios::binary);
	
	//3.print the decoded message
	// In the checkpoint we need to change a line because after we read 256 integer, the posi will hold at the change line symbol
	//in.get();
	//In the final submission, we use a counter to count the number of characters that have been decoded, with the limitation of the totalNum
	int alreadyDecodeNum = 0;
	while(alreadyDecodeNum < totalNum){
		//cout<<"enter the while loop"<<endl;
		int letter_int = tree.decode(bitIn);
		//cout<<"pass the first decoding"<<endl;
		if(in.eof()) break;
		//cout<< letter_int <<endl;
		if(letter_int == -1) break;
		out<<(byte)letter_int;
		alreadyDecodeNum++;
	}
	//out<<"output finished";
	out.close();
	in.close();

	return 0;
}