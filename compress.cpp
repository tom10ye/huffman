#include "HCNode.h"
#include "HCTree.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
	char* infile = argv[1];
	char* outfile = argv[2];

	vector<int> freqs(256,0);

	ifstream in;
	ofstream out;

	in.open(infile, ios::in | ios::binary);

	byte letter_byte;

	// A bug arise here in in.eof()
	// if it is the parameter of 
	while(true){
		in>>letter_byte;
		if(in.eof()) break;
		int idx = (int)letter_byte;
		freqs[idx] =  freqs[idx] + 1;
	}

	in.close();

	//build the tree
	HCTree tree;
	tree.build(freqs);
    
    in.open(infile, ios::in | ios::binary);
	out.open(outfile, ios::out | ios::binary);
	
	//print header as lines of freqs[]
	//the line index is the ascii code, and the line itself is the frequency number of this ascii number
	for(int i = 0; i<256 ;i++){
		out<<freqs[i]<<endl;
	}
	//print the encoded message
	//out<<"print the encoded message"<<endl;
	in>>letter_byte;
	while(!in.eof()){
		//if(in.eof()) break;
		tree.encode(letter_byte, out);
		//out<<" "<<endl;
		in>>letter_byte;
	}
	out.close();
	in.close();

	return 0;
}