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
	BitOutputStream bitOut(out);//instantiate BitOutputStream with the initial parameter ofstream

	in.open(infile, ios::in | ios::binary);

	byte letter_byte;
	vector<int> freqs(256,0);//for store a statistic of 256 ascii symbols

	// A bug arise here in in.eof()
	// if it is the parameter of while loop, it will finally cause repeat of the last word, the exact situation is in the report checkpoint
	//2.Statistic 
	while(true){
		letter_byte = in.get();
		//cout<<letter_byte<<endl;
		if(in.eof()) break;
		int idx = (int)letter_byte;
		//cout<<idx<<endl;
		freqs[idx] =  freqs[idx] + 1;
	}

	in.close();

	//3.build the tree
	HCTree tree;
	tree.build(freqs);
    
    in.open(infile, ios::in | ios::binary);
	out.open(outfile, ios::out | ios::binary);
	

	//4.make header
	//NAIVE HEADER
	//print header as lines of freqs[]
	//the line index is the ascii code, and the line itself is the frequency number of this ascii number
	/*
	for(int i = 0; i<256 ;i++){
		//out<<"write the frequency"<<endl;
		//for(int i=0 ;i<9;i++) bitOut.writeBit(1);
		int freq_int = freqs[i];
		for(int j = 0; j<32; j++){
			int bit = (freq_int >> (31-j)) & 1;// here we do not change line after a freq has been write
			//cout<<bit;
			bitOut.writeBit(bit);
		}
		//cout<<endl;
	}
	*/

	//CLEVER HEADER
	//In the clever header, we first need to record the total number of symbols and unique symbols in our header
	//the total number is used to limit the reading of body not beyond to the flushed 0s
	//the unique number is used to limit the reading of header part
	//And then record each leaf node's symbol and its depth from left to right (it was just like pre/in/post traverse the tree and remove the non-leaf nodes)
	//At the decoder. use a specific algorithm to rebuild the tree through this sequence
	//Why can we serialize a tree like this? I have done a lot of research and serilization a binary tree is even a hard problem on leetcode
	//Becasue huffman tree is a full tree. It does not tolerate a node with only one child.
	//Thus we can use this very specific algorithm to rebuild the tree only by the sequence and depth of the leaf node
	//I will illustrate reconstruction algorithm in the corresponding part.

	int totalNum = 0;
	int uniqueNum = 0;
	for(int i = 0; i < 256; i++){
	    if(freqs[i] != 0){
	      uniqueNum = uniqueNum + 1;
	      totalNum = totalNum + freqs[i];//record the total symbol number to deal with the flush bug
	    }
	}
	//cout<<"totalNum is "<<totalNum<<endl;
	bitOut.writeInt(totalNum);
	//bitOut.flush();
	bitOut.writeByte(uniqueNum); //here we only need a byte to store the unique symbols number, because it will not go beyond 256 for ascii code
	//out<<"finish write the total unique number"<<endl;
	tree.printCleverHeader(bitOut);


	//5.print the encoded message
	while(true){
		letter_byte = in.get();
		if(in.eof()) break;
		tree.encode(letter_byte, bitOut);
	}

	//6.clear the rest
	//Imagine that if you only have two symbols, which encoded as 0 and 1, then the two bit could not make up a byte, so you need to mannually flush the buffer at the end.
	//How can we not decode the flushed 0s?
	//we use the total number of symbols to limit the time of decode operations
	bitOut.flush();

	out.close();
	in.close();

	return 0;
}