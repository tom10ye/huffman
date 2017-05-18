#include "HCNode.h"
#include "HCTree.h"
#include "BitInputStream.h"
#include "BitOutputStream.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
	char* infile = argv[1];
	char* outfile = argv[2];

	vector<int> freqs(256,0);

	ifstream in;
	ofstream out;
	//final submission add on
	BitOutputStream bitOut(out);

	in.open(infile, ios::in | ios::binary);

	byte letter_byte;

	// A bug arise here in in.eof()
	// if it is the parameter of 
	while(true){
		letter_byte = in.get();
		//cout<<letter_byte<<endl;
		if(in.eof()) break;
		int idx = (int)letter_byte;
		//cout<<idx<<endl;
		freqs[idx] =  freqs[idx] + 1;
	}

	in.close();

	//build the tree
	HCTree tree;
	tree.build(freqs);
    
    in.open(infile, ios::in | ios::binary);
	out.open(outfile, ios::out | ios::binary);
	
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
	bitOut.flush();
	bitOut.writeByte(uniqueNum);
	//out<<"finish write the total unique number"<<endl;
	tree.printCleverHeader(bitOut);


	//print the encoded message
	while(true){
		letter_byte = in.get();
		if(in.eof()) break;
		tree.encode(letter_byte, bitOut);
	}

	//clear the rest
	bitOut.flush();

	out.close();
	in.close();

	return 0;
}