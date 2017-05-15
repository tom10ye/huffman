#include "HCTree.h"
#include <stack>
using namespace std;

HCTree::~HCTree(){
	deleteAll(root);
}

void HCTree::deleteAll(HCNode* root){
	if(root == NULL) return;
	deleteAll(root->c0);
	deleteAll(root->c1);
	delete root;
}

void HCTree::build(const vector<int>& freqs){
	
	std::priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> PQ;
	for(int i=0; i<freqs.size();i++){
		if(freqs[i] != 0){
			HCNode* node = new HCNode(freqs[i],(byte) i);
			PQ.push(node);
			leaves[i] = node;
		}
	}

	//while(!PQ.empty()){
	//leave one node in the PQ, because the root of the tree has no sense
	while(PQ.size()>1){
		HCNode* node0 = PQ.top();
		PQ.pop();
		HCNode* node1 = PQ.top();
		PQ.pop();
		HCNode* node = new HCNode(node0->count + node1->count, node0->symbol, node0, node1);
		node0->p = node;
		node1->p = node;
		PQ.push(node);
	}

	root = PQ.top();
	PQ.pop();
}

void HCTree::encode(byte symbol, ofstream& out) const{
	HCNode* target = leaves[(int)symbol];
	if(target == root){
		out<<"0";
		return;
	}
	if(target->count == 0) return; 

	stack<char> stack;

	while(target->p != NULL){
		if(target->p->c0 == target){
			stack.push('0');
		}else{
			stack.push('1');
		}
		target = target->p;
	}

	while(!stack.empty()){
		out<<stack.top();
		stack.pop();
	}
}

int HCTree::decode(ifstream& in) const{
	HCNode* node = root;
	while(node->c0 != NULL){//huffman tree node must have either no child or two children
		char out_char;
		out_char = in.get();
		if(out_char == '0'){
			node = node->c0;
		}else if(out_char == '1'){
			node = node->c1;
		}else return -1;
	}
	return (int)node->symbol;
}

//final submission

void HCTree::encode(byte symbol, BitOutputStream& out) const{
	HCNode* target = leaves[(int)symbol];
	if(target == root){
		out.writeBit(0);
		return;
	}
	if(target->count == 0) return; 

	stack<char> stack;

	while(target->p != NULL){
		if(target->p->c0 == target){
			stack.push('0');
		}else{
			stack.push('1');
		}
		target = target->p;
	}

	while(!stack.empty()){
		out.writeBit(stack.top()-'0');
		stack.pop();
	}
}

int HCTree::decode(BitInputStream& in) const{
	HCNode* node = root;
	while(node->c0 != NULL){//huffman tree node must have either no child or two children
		int bit_int = in.readBit();
		if(bit_int == 0){
			node = node->c0;
		}else if(bit_int == 1){
			node = node->c1;
		}else return -1;
	}
	return (int)node->symbol;
}