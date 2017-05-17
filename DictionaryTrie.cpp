/*
Author: Cheng Qian
Project: UCSD CSE100 PA2
Version: 3.5.2 (For PA3 code refactor)
*/

#include<malloc.h>
#include "util.h"
#include "DictionaryTrie.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include<string>
#include<stdlib.h>
using namespace std;  


//1.Dictionary Trie Constructor
DictionaryTrie::DictionaryTrie(){
	root = new TrieNode();
}


//2.TrieNode constructor
TrieNode::TrieNode(){
	frequency = 0;
	end = false;
	for(int i =0; i<27;i++) next[i] = NULL; //fill the vector with null ptr
}


//3.TrieNode deconstructor
TrieNode::~TrieNode(){}


/*4.Insert
 * Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string) */
bool DictionaryTrie::insert(std::string word, unsigned int freq)
{
	if(!root){
		root = new TrieNode();
	}
	TrieNode* node = root;
	bool success;
	char *p = &word[0];//initialize a pointer to the first character of word
	int idx = 0;
	while( *p){
		idx = *p - 'a';
		if(idx == -65) idx = 26; // if current charater is space, then give the last index to it
		if(node->next[idx] == NULL){
			node->next[idx] = new TrieNode();
		//@original
		// 	success = true;
		// }else{
		// 	success = false;
		//
		}
		node = node->next[idx];
		++p;
	}
	node->frequency = std::max(freq, node->frequency);//update the frequency to be the max value between original frequency of the node and new one
	//@rewrite
	if(node->end == false) {
		success = true; 
		node->end = true;//set the end flag of this node to be true
	}else
		success = false;
	return success;
}


/*5.Find
  Return true if word is in the dictionary, and false otherwise */
bool DictionaryTrie::find(std::string word) const
{
	TrieNode* node = this->root;
	char *p = &word[0];
	int idx = 0;

	while( *p ){
		idx = *p - 'a';
		if(idx == -65) idx = 26; // if p is space, then give the last index to it
		node = node->next[idx];
		++p;
		if(node == NULL) return false;
	}
	if (node->end == false) return false;
	return true;
}


/*6.predictCompletions
 * Return up to num_completions of the most frequent completions
 * of the prefix, such that the completions are words in the dictionary.
 * These completions should be listed from most frequent to least.
 * If there are fewer than num_completions legal completions, this
 * function returns a vector with as many completions as possible.
 * If no completions exist, then the function returns a vector of size 0.
 * The prefix itself might be included in the returned words if the prefix
 * is a word (and is among the num_completions most frequent completions
 * of the prefix)
 */
std::vector<std::string> DictionaryTrie::predictCompletions(std::string prefix, unsigned int num_completions)
{
	std::vector<std::string> words;

	//Check if the MWT is empty
	if(root == NULL) return words;


	//User Input validation(check NULL input and 0 return number)
	if((prefix == "") || (num_completions <=0)){
		cout<< "Invalid Input. Do not input null prefix or zero number. Please retry with correct input."<<endl;
		return words;
	}
	

	//Original: validatation and locate subtrie root together
	/*
	char *p = &prefix[0];
	int idx = 0;
	TrieNode* node = this->root;
	while(*p){
		idx = *p - 'a';
		if((idx == -65) || (idx>=0 && idx<=25)){
			node = node->next[idx];
			if(node == NULL) return words;		
			++p;
			//continue;
		}else{
			cout<< "Invalid Input. Please retry with correct input."<<endl;
			return words;
		}
	}
	*/


	//New: Modulized different fuctional parts
	//User Input validation(Invalid letter, must be a to z or space)
	char *p = &prefix[0];
	int idx = 0;
	while(*p){
		idx = *p - 'a';
		if((idx == -65) || (idx>=0 && idx<=25)){
			++p;
			//continue;
		}else{
			cout<< "Invalid Input. Please retry with correct input."<<endl;
			return words;
		}
	}

	//New: Modulized different fuctional parts
	//First we need to locate the subTree's root to begin our traversal
	p = &prefix[0];
	TrieNode* node = this->root;
	while(*p){
		idx = *p -'a';
		if(idx == -65) idx = 26; // if p is space, then give the last index to it
		node = node->next[idx];
		++p;
		if(node == NULL) return words;		
	}


	//priority_queue<Type, Container, Functional>
	std::priority_queue<std::pair<TrieNode*, std::string>, std::vector<std::pair<TrieNode*, std::string>>, TrieNodeComparator> PQ;// use for track the most frequent word

	std::queue<std::pair<TrieNode*,std::string>> Q;//use for BFS traversal, the members are pairs. 
	//The first parameter is a ptr to the TrieNode and the seconde parameter is a string consist of all the letters along the path from subtrie's root to the current node.
	//In this case, we can just add the newletter to the original string and finally add prefix in the front of it.

	std::pair<TrieNode*, std::string> currPair(node,"");//the sub-trie root pair

	//BFS traverse the subTrie and push all the encounter node into priorityqueue
	Q.push(currPair);
	while(!Q.empty()){
		currPair = Q.front();
		Q.pop();
		if(currPair.first->end == true){
			PQ.push(currPair);// the prefix itself is also a candidate of result
			//@New
			if(PQ.size() > num_completions) PQ.pop(); // to maintain a PQ with capacity of the num_completions that the user want
		
		}
		for(int i = 0; i<27; i++){
			if(currPair.first->next[i] != NULL){
				char addOnLetter;
				if(i == 26){
					addOnLetter = ' '; // 26 ---- space
				}else{
					addOnLetter = i + 97;
				}
				std::string addOnWord = currPair.second + addOnLetter; // add the letter to the original string
				std::pair<TrieNode*, std::string> nextPair(currPair.first->next[i], addOnWord);
				Q.push(nextPair);
			}
		}
	}

	//pop out the pair from priority queue and store the words in a vector
	//@Change
	//while(PQ.size>num_completions)
	while(!PQ.empty()){
		std::string newWord = prefix + PQ.top().second;
		words.push_back(newWord);
		PQ.pop();		
	}

	//because the first node popped out from PQ is the least frequent, we need to reverse the sequence  
	reverse(words.begin(), words.end());
	return words;
}


/* 7.DictionaryTrie Destructor */
DictionaryTrie::~DictionaryTrie(){
	deleteAll(root);
}


/*8 deletAll (A helper function for DictionaryTrie Deconstructor)*/
void DictionaryTrie::deleteAll(TrieNode* root){
	if(root == NULL) return;
	for(unsigned int i = 0; i < 27; i++){
		deleteAll(root->next[i]); //recursively delete all the child nodes before deleting parent node
	}
	delete root;
}
