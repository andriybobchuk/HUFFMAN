/** @file functions.cpp */

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

 // standard libraries
#include <iostream> // input / output operations
#include <fstream> // operatioons with file streams
#include <string> // char strings
#include <vector> //operations with vectors
#include <map> //operations with maps
#include <list> //operations with lists
#include <sstream> //stringstreams


// my own header files
#include "functions.h" // declarations of functions

bool IfFileExists(const std::string& name) {
	ifstream file(name.c_str());
	return file.good();
}


bool empty(const std::string& name)
{
	ifstream file(name.c_str());
	return file.peek() == std::ifstream::traits_type::eof();
}


void error()
{
	cout << "|======================= ERROR!! =======================|" << endl;
	cout << "| You made a mistake typing command line arguments!     |" << endl;
	help();
	cout << "| NOTE: You may pass arguments in ANY order             |" << endl;
	cout << "|=======================================================|" << endl;
}

void help()
{
	cout << "| Tutorial:                                             |" << endl;
	cout << "| type '-i' to specify the input file;                  |" << endl;
	cout << "| type '-o' to specify the output file;                 |" << endl;
	cout << "| type '-d' to specify the dictionary file;             |" << endl;
	cout << "| type '-m' to specify the mode:                        |" << endl;
	cout << "|                                'c' to compress;       |" << endl;
	cout << "|                                'd' to decompress.     |" << endl;
}

/*This is for converting string to vector
(I need it in function DECOMPRESS)*/
auto stringToVector(string&& s) {

	vector<bool> v;//vector we want to get after converting

	for (auto i : s) //for char in string
		v.push_back(i == '1');//add it to a vector as a separate element

	return v;//Our final vector!
}

/*=======================================================================================
 * ENCODE part of the program ↓
 * Algorithm:
 * 1) Count the frequencies of each char in input file using associative array "MAP"
 * 2) Based on the frequencies of chars write nodes with one-named fields to the list
 * 3) Create the Huffman tree summing frequencies and sorting list of nodes
 * 4) Generate a dictionary
 * 5) Compress the file 
 *=====================================================================================*/

//Create pairs "Character-Code" function
void DictionaryGenerator(Node* root, map<char, vector<bool> > (&dict), vector<bool> (&code))
{
	if (root->left != NULL)//If it is on the left side
	{
		code.push_back(0);//Then we write '0'
		DictionaryGenerator(root->left, dict, code);//Again - start recursion function BuildTable but now for left child
	}

	if (root->right != NULL)//If it is on the right side
	{
		code.push_back(1);//Then we write '1'
		DictionaryGenerator(root->right, dict, code);//Again - start recursion function BuildTable but now for right child
	}

	/*If we reached the character we have to assign a binary code to it:*/
	if (root->left == NULL && root->right == NULL) dict[root->character] = code;


	if (!code.empty()) {
		code.pop_back();//code -1
	}

	//After this func execution I'll write the dict table to a file
}


void countFrequencies(string In, map <char, int> & freqTable)
{
	/*=======================================================================================
     * Here we count the frequency of each char in file using associative array "MAP"
     *=====================================================================================*/

	ifstream inFile(In, ios::in | ios::binary);//Open file for binary reading

	while (!inFile.eof())//While Not end of file
	{
		char characterToRead = inFile.get();//reading file with spaces

		freqTable[characterToRead]++;//add 1 char to frequencies table
		//It just assigns and increments the counter to the found char
	}
	inFile.close();
}


void fillNodeList(string Dict, map <char, int> & freqTable, list<Node*> & nodePointersList)
{
	/*=======================================================================================
	 * Here we write start nodes to our list & fill the dict
	 *=====================================================================================*/

	ofstream DictFile(Dict, ios::out | ios::binary);//open file in binary mode
	DictFile.clear();


	/**From begin of Map to the end of Map*/
	for (map <char, int>::iterator itr = freqTable.begin(); itr != freqTable.end(); ++itr)
	{
		/* Save frequencies to the 'Dictionary' file */
		DictFile << (int)(*itr).first << ' '; //Prints ii-th char from the map
		DictFile << (*itr).second << endl; //Prints ii-th char's frequency from the map

		//Debug ↓
		//cout << (int)(*itr).first << ' '; //Prints ii-th char from the map
		//cout << (*itr).second << endl; //Prints ii-th char's frequency from the map

		/*Put elements FROM map INTO our list as nodes*/

		Node* p = new Node;//Create pointer at node
		p->character = itr->first;//Field of class NODE "character" becomes first 
		p->freq = itr->second;//Field of class NODE "frequency" becomes second 
		nodePointersList.push_back(p);//add this pointer (which include char & frequency) to our list
	}
	DictFile.close();// Close the DICTIONARY file
}



void createHuffmanTree(list<Node*> & nodePointersList)
{
	/*=======================================================================================
	 * Here we create the Huffman's tree
	 *=====================================================================================*/

	while (nodePointersList.size() != 1)//While in our list is NOT 1 element
	{
		/*The last iteration of loop will give us one single element -
		root of the tree*/

		nodePointersList.sort(compare());//sort list using compare structure

		/*We need 2 nodes to sum them:*/
		Node* leftChild = nodePointersList.front();//GET THE FIRST ELEMENT from the beginning of nodePointersList
		nodePointersList.pop_front();//Delete copied element from nodePointersList
		Node* rightChild = nodePointersList.front();//GET THE NEXT ELEMENT from the beginning of nodePointersList
		nodePointersList.pop_front();//Delete copied element from nodePointersList

		/*Here we sum those 2 elements:*/
		Node* Parent = new Node(leftChild, rightChild);//Create a parent (childrens' frequencies will be added) 

		nodePointersList.push_back(Parent);	//and put it on the list as a new node.

		//now go to sort again
		//until we have just one root element
	}
}


void compress(string In, string Out, string Dict)
{
	/* === Declaration part === */
	list<Node*> nodePointersList;//list of pointers at nodes(which have freq and char taken from the map of frequencies)
	Node* root;// pointer at the top of the tree
	map<char, vector<bool> > dict;// associative array of charater and its binary code
	map <char, int> freqTable;//associative array where 1st element is a character, 2nd - its frequency
	vector<bool> code;// Binary codes of chars
	/* === /Declaration part === */

	countFrequencies(In, freqTable);

	fillNodeList(Dict, freqTable, nodePointersList);

	createHuffmanTree(nodePointersList);
	root = nodePointersList.front();

	// Create a dictionary
	DictionaryGenerator(root, dict, code);//Create pairs "Character-Code". Other words - Huffman Dictionary


	/*=======================================================================================
	 * Here we create & write our binary (Compressed) code to Output file
	 *=====================================================================================*/

	ifstream inFile(In, ios::in | ios::binary);//Open file for binary reading
	inFile.clear();
	inFile.seekg(0); //switch our cursor to the beginning of the INPUT file (to read it from the beginning)

	ofstream outFile(Out, ios::out | ios::binary);//open OUTPUT file for writting in binary mode

	int count = 0; // Counter in range 0-8 to count when the new byte starts
	char buf = 0; // Here we store our 8 bits which were set during 1 counter loop (0-8) 

	while (!inFile.eof()) // While not end of INPUT file (We read the input file)
	{
		char c = inFile.get(); //Take char from the file

		vector<bool> x = dict[c]; //look for its binary code in the dictionary

		/*This is how I “pack” my stream of zeros and ones into bytes*/
		for (int n = 0; n < x.size(); n++)
		{
			/* Write code into the Buffer variable. At the beginning buf=00000000 and then with
			each new iteration we copy the value from the dictionary to our buffer (using binary
			addition)*/
			/*               << means SHIFT element by (7-count). We do that in oder to
			read bits correctly left to right*/
			buf = buf | x[n] << (7 - count);// '|' means binary addition; x[n] is our current bit
			count++;

			/* if we passed 8 bit, that means we got 1 byte. Than we write that obtained byte
			from the buffer byte (buf) to the Output File. */
			if (count == 8) { count = 0;   outFile << buf; buf = 0; }

			/*In the end we will get approximately 50% lighter file with non-sence characters.
			By the way, this file may not even have the extension! It can be either BIN or TXT,
			doesn't matter. */
		}
	}

	/*Close all of the files (DICTIONARY file is already closed) */
	inFile.close();// Close the INPUT file
	outFile.close();// Close the OUTPUT file

}


/*=======================================================================================
 * DECODE part of the program ↓
 * Algorithm:
 * 1) Convert dictionary file to a map <int, int>
 * 2) Based on the frequencies of chars (step 1) write nodes with one-named fields to the list
 * 3) Create the Huffman tree summing frequencies and sorting list of nodes
 * 4) Decode the input file & write to output file 
 *=====================================================================================*/

void fillNodeList_MODE_DECODE(string Dict, map <int, int>& freqTable, list<Node*>& nodePointersList)
{
	/*=======================================================================================
	 * Here we write start nodes to our list & fill the dict
	 *=====================================================================================*/

	ofstream DictFile(Dict, ios::out | ios::binary);//open file in binary mode
	DictFile.clear();

	/**From begin of Map to the end of Map*/
	for (map <int, int>::iterator itr = freqTable.begin(); itr != freqTable.end(); ++itr)
	{
		/* Save frequencies to the 'Dictionary' file */
		DictFile << (int)(*itr).first << ' '; //Prints ii-th char from the map
		DictFile << (*itr).second << endl; //Prints ii-th char's frequency from the map

		/*Put elements FROM map INTO our list as nodes*/

		Node* p = new Node;//Create pointer at node
		p->character = itr->first;//Field of class NODE "character" becomes first 
		p->freq = itr->second;//Field of class NODE "frequency" becomes second 
		nodePointersList.push_back(p);//add this pointer (which include char & frequency) to our list
	}
	DictFile.close();// Close the DICTIONARY file
}


void decompress(string In, string Out, string Dict)
{
	/* Instead of very complicated and SLOW(!) decompress method by checking each 
	bit with the dictionary I just decided to RESTORE THE HUFFMAN TREE using the 
	frequency dictionary. 
	Representing the Huffman code as a binary tree, and walking this tree to decode is a 
	KEY to superspeed!! */

	/* === Declaration part === */
	list<Node*> nodePointersListAgain;//list of pointers at nodes(which have freq and char taken from the map of frequencies)
	Node* rootAgain;// pointer at the top of the tree
	/* === /Declaration part === */

	/*=======================================================================================
	 * Convert dictionary file to a map <int, int>
	 *=====================================================================================*/

	ifstream Dictionary(Dict, ios::in | ios::binary);//Open the frequencies dictionary file

	map<int, int> dict;//map in which we store int of character and its frequency
	std::string line;//line to read
	int character, frequency;//fields of our future dictionary

	/* I don't use read here because dictionaries are small files as a rule */
	while (std::getline(Dictionary, line))
	{
		std::istringstream iss(line);
		iss >> character;
		iss >> frequency;
		dict[character] = frequency;
	}
	Dictionary.close();


	//Create node (char & frequency)
	fillNodeList_MODE_DECODE(Dict, dict, nodePointersListAgain);

	//Create a tree
	createHuffmanTree(nodePointersListAgain);

	//get root
	rootAgain = nodePointersListAgain.front();


	/*=======================================================================================
	 * Decode the input file & write to output file 
	 *=====================================================================================*/

	ifstream inFile(In, ios::in | ios::binary);//open INPUT file 
	ofstream outFile(Out, ios::out | ios::binary);//open OUTPUT file 

	Node* p = rootAgain;//received root
	int count = 0; 
	char byte;
	byte = inFile.get();//read file
	  
	while (!inFile.eof())// not end of input (encoded) file
	{
		bool b = byte & (1 << (7 - count));//take BITS
		if (b) //if TRUE (true=='1')
			p = p->right; //go right
		else //if 0
			p = p->left;//left

		if (p->left == NULL && p->right == NULL) //if this is the last node (no children)
		{ 
			outFile << p->character;//print its char
			p = rootAgain; //start from the beginning
		}
		count++;
		/*This is how I take BYTES*/
		if (count == 8)
		{ 
			count = 0; 
			byte = inFile.get(); 
		}
	}

	inFile.close();
	outFile.close();

}