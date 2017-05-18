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

	stack<unsigned char> stack;

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
		unsigned char out_char;
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

	stack<unsigned char> stack;

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
		//cout<<"enter the decode loop"<<endl;
		int bit_int = in.readBit();
		if(bit_int == 0){
			node = node->c0;
		}else if(bit_int == 1){
			node = node->c1;
			//cout<<"node goes to right"<<endl;
			//if(node == NULL) cout<<"node is NULL!"<<endl;
		}else return -1;
	}
	return (int)node->symbol;
}
//CleverHeader format:
//xx4(totalNumber for 4byte)xx(uniqueNumber for 1byte)
//xx(symbol for 1 byte)xx(depth for 1 byte)
// here we encode depth for 1 byte because in ascii coding there will not be a deeper tree more than 256
void HCTree::printCleverHeader(BitOutputStream& out) const{
	helper_traverse(root,0,out);
}

void HCTree::helper_traverse(HCNode* node, int depth, BitOutputStream& out) const{
    if(node == nullptr) return;
    if(node->c0 == nullptr){ // leaf 
      out.writeByte(node->symbol);
      out.writeByte(depth);
      return;
    }
    // always take left before right, you can think it as pre/in/post order traverse the tree and remove all the non-leaf nodes
    helper_traverse(node->c0, depth+1, out);	
	helper_traverse(node->c1, depth+1, out); 
}


//This function actually read and build the tree
//The algorithm is that always go left as far as the depth regulating us, and simutaneously intialize the nodes on the path,
//After reaching the leaf, store the symbol and pop up to a node whose right child is NULL
void HCTree::readCleverHeader(BitInputStream& in, int uniqueNum) {
	
	root = new HCNode(0,0);
	int currDepth = 0;
	HCNode* curr = root;
	HCNode* child;

	for(int i = 0; i < uniqueNum; i++){
	    

	    byte symbol = in.readByte();
	    int depth = in.readByte();		
	    //cout<<"target depth is "<< depth<<endl;

	    //here, after one iteration, the new coming node's depth will  be definitely currDepth(the node without a right child)+1, because we always need to make up the right child to keep the huffman tree full
	    //The proof is that we need to make this tree full, which means we do not tolerant a node with only one child
	    while(currDepth < depth){
	    	child = new HCNode(0,0,NULL,NULL,curr);
	    	if(curr->c0 == NULL){
	    		curr->c0 = child;
	    		curr = curr->c0;
	    	}else{
	    		curr->c1 = child;
	    		curr = curr->c1;
	    	}
	    	currDepth = currDepth +1;
	    }
	    curr->symbol = symbol;

	    //first we need to pop up to a position whose parent's c1 child is null
	    while((curr->p != NULL) && (curr->p->c1 != NULL)){
	    	curr = curr->p;
	    	currDepth = currDepth - 1;
	    }


	    //if there is only one node in this tree, we do not need to move the ptr
	    //other wise make the curr ptr point to the parent of the position we have got to
	    if(curr->p != NULL){
		    curr = curr->p;
		    currDepth = currDepth - 1;
		}
	}

}