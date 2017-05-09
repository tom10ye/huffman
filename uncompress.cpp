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

	//read  header
	int frequency_int;
	for(int i = 0; i < 256; i++){
		in>>frequency_int;
		freqs[i] = frequency_int;
	}

	//build tree
	HCTree tree;
	tree.build(freqs);

	out.open(outfile, ios::out | ios::binary);
	
	//print the decoded message

	while(!in.eof()){
		//out<<"enter the while loop"<<endl;
		int letter_int = tree.decode(in);
		if(in.eof()) break;
		//out<< letter_int <<endl;
		if(letter_int == -1) break;
		out<<(byte)letter_int;
	}
	//out<<"output finished";
	out.close();
	in.close();

	return 0;
}