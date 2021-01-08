/** @file functions.h */
/* functions header file */

/*
FUNDAMENATALS OF COMPUTER PROGRAMMING

 Exam

 Andrii Bobchuk
 andrbob116@student.polsl.pl
 https://andriybobchuk.com

 * Date created:       18.10.2020
   Task:               Huffman
   language:           C++
 */

 //#pragma once

 // prevention of multiple includes
#include <string>
#ifndef FUNCTIONS_H  // if FUNCTIONS_H is not defined ...
#define FUNCTIONS_H  // ... then define  FUNCTIONS_H
                     // otherwise jump to #endif 

using namespace std;//Not to write std:: each time


/** Class Node with fields 'frequency', 'character' and the pointers for 
children*/
class Node
{
public:
	int freq; ///< Quantity of the same character(frequency)
	char character;///< Character
	Node* left, * right;///< Pointers at the left and right child

	Node() { left = right = NULL; }

	//constructor which gets two links - on Left & Right Child
	Node(Node* L, Node* R)
	{
		left = L;
		right = R;
		freq = L->freq + R->freq;//So we just add frequencies of two children
	}
};


/** Structure which compares the nodes in the list to sort them */
struct compare//for sorting the list
{
	//returns true is left parameter is smaller than the right parameter
	bool operator()(const Node* l, const Node* r) const { 
		return l->freq < r->freq; //sorting pointers
	}
};


/** Function checks the accesibility of the file
  @param s is name of the string to be checked
  @return Function returns true is the file is accesible
*/
bool IfFileExists(const std::string& name);


/** Function checks if the file is empty
  @param s is name of the string to be checked
  @return Function returns true if the file is empty
*/
bool empty(const std::string& name);


/** Function for printing error & help message
  @return Function returns nothing, thus "void" return type.
*/
void error();


/** Function for printing help message
  @return Function returns nothing, thus "void" return type.
*/
void help();


/** Function for converting string to vector
  @param s is name of the string to be converted
  @return Function returns nothing
*/
auto stringToVector(string&& s);


/** Function for counting frequencies of characters
  @param In is name of the -i(input (Compressed) file )
  @param map is the container in which we store data
  @return Function returns nothing, thus "void" return type.
*/
void countFrequencies(string In, map <char, int>& freqTable);


/** Function for filling the node list with the new nodes
  @param In is name of the -i(input (Compressed) )
  @param Out is the name of the -o(output (Decompressed) )
  @param Dict is the name of the dictionary file which will be used to decompress file
  @return Function returns nothing, thus "void" return type.
*/
void fillNodeList(string Dict, map <char, int>& freqTable, list<Node*>& nodePointersList);


/** Function for summing frequencies of nodes and sorting them
  @param nodePointersList is name of node list)
  @return Function returns nothing, thus "void" return type.
*/
void createHuffmanTree(list<Node*>& nodePointersList);


/** Function for filling the node list with the new nodes BUT in case of decompressing
  @param Dict is name of the dictionary file
  @param map is the name of the container to store frequencies
  @param nodePointersList is the list of nodes
  @return Function returns nothing, thus "void" return type.
*/
void fillNodeList_MODE_DECODE(string Dict, map <int, int>& freqTable, list<Node*>& nodePointersList);


/** Function for decompressing the file compressed file.
  @param In is name of the -i(input (Compressed) )
  @param Out is the name of the -o(output (Decompressed) )
  @param Dict is the name of the dictionary file which will be used to decompress file
  @return Function returns nothing, thus "void" return type.
*/
void DictionaryGenerator(Node* root);


/** Function for compressing the file.
  @param In is the name of the -i(input) file to be compressed
  @param Out is the name of the -o(output) file to be compressed
  @param Dict is the name of the dictionary file which compress function will generate
  @return Function returns nothing, thus "void" return type.
*/
void compress(string In, string Out, string Dict);


/** Function for decompressing the file compressed file.
  @param In is name of the -i(input (Compressed) )
  @param Out is the name of the -o(output (Decompressed) )
  @param Dict is the name of the dictionary file which will be used to decompress file
  @return Function returns nothing, thus "void" return type.
*/
void decompress(string In, string Out, string Dic);


#endif  
