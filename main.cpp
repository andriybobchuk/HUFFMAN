/** @file main.cpp */

/*
 FUNDAMENATALS OF COMPUTER PROGRAMMING

 Exam

 Andrii Bobchuk
 andrbob116@student.polsl.pl
 https://andriybobchuk.com

 * Date created:       18.10.2020
   Task:               Huffman
   language:           C++

 * Description
   David Huffman was the person who invented one of the most efficient approaches to 
   compressing files (mostly text ones) using the rule: "THE MORE A CHARACTER IS MENTIONED, THE SHORTER 
   ITS SIZE SHOULD BE". 
   A normal char takes 1 byte or 8 bit to encode it according to ASCII table, but Huffman desided to decrease the 
   size of the most frequent characters. It caused one HUGE problem - reading and recognizing symbols. When
   computer wants to translate binary code into a normal text it separates the whole code by 8 digits to convert
   those small groups into letters, spaces, special sybols etc. but according to the Huffman, all characters 
   has a variable bit code size, how to read it the right way then?

   The answer is breath-taking - You cannot read the Huffman code the wrong way! It is a prefix code which means 
   none of the codeword is the beginning of another codeword. And it's all thanks to the tree system which assigns
   that codes to characters(By the way - the shortest possible codes)


   This program is an implementation of his algorithm in C++

   Release notes:
   * Got rid of ALL global variables
   * Use flipped map (<vector<bool>, int>) to implement find()
   * Store characters as ints not to have troubles with reading
   * Splitted program code to simple functions
   * Any permutation of command-line switches are handled (user mistakes too).
   * Speed of execution is way better now
   * Changed the way of decoding to recreation tree and walking trough it

 */

// standard libraries
#include <iostream> // input / output operations
#include <fstream> // operatioons with file streams
#include <string> // char strings
#include <vector> //operations with vectors
#include <map> //operations with maps
#include <list> //operations with lists

// my own header files
#include "functions.h"    // declarations of functions
// I do not include *.cpp files!
// *.cpp files are compiled, *.h files are included

using namespace std;//import this namespace in order not to write 'std::' all the time

int main(int argc, char* argv[])
{
	cout << " |  | |  | |*** |*** |*  *|   /*   |*   | " << endl;
	cout << " |--| |  | |*** |*** | ** |  /__*  | *  | " << endl;
	cout << " |  | |__| |    |    |    | /    * |  * | " << endl << endl;

	string InFile; //Name of the Input file
	string OutFile; // Name of the Output file 
	string Dictionary; // Name of the Dictionary file 
	string Mode; // Mode: Compress or Decompress 

	/*=======================================================================================
	 * Here we recognize the command line arguments
	 *=====================================================================================*/

	if (argc == 9)//the user MUST enter 8 arguments 
	{
		for (int i = 0; i <= 8; i++)//We use the for loop to read the args
		{
			if (!strcmp(argv[i], "-i"))// If we detected argument "-i"
			{
				InFile = argv[i + 1];//We assume that after -i flag goes an Input File name -> catch it to a variable
			}
			if (!strcmp(argv[i], "-o"))// If we detected argument "-o"
			{
				OutFile = argv[i + 1];//We assume that after -o flag goes an Output File name -> catch it to a variable
			}
			if (!strcmp(argv[i], "-d"))// If we detected argument "-d"
			{
				Dictionary = argv[i + 1];//We assume that after -d flag goes a Dictionary File name-> catch it to a variable
			}
			if (!strcmp(argv[i], "-m"))//We assume that after -m flag goes a mode (c or d) 
			{
				/**Now we will determine the mode:*/
				if (!strcmp(argv[i + 1], "c"))// If we detected argument "c"
				{
					Mode = "COMPRESS";
				}
				if (!strcmp(argv[i + 1], "d"))// If we detected argument "d"
				{
					Mode = "DECOMPRESS";
				}
			}
		}
	}
	else if (argc == 2)//0th is the name and 1st is the -h
	{
		if (!strcmp(argv[1], "-h"))// If we detected argument "-h" 
		{
			help();
			return 0; //Finish the program execution
		}
		else
		{
			error();
			return 0; //Finish the program execution
		}
	}
	else if (argc == 1)//0th - prog name
	{
		help();
		return 0; //Finish the program execution
	}
	else//if other quantity of params
	{
		/*=== Error mesaage and the instructions ===*/
		error();
		return 0; //Finish the program execution
	}

	/*=== Print the user's choice =========================*/
	cout << "Input File: " << InFile << endl;
	cout << "Output File: " << OutFile << endl;
	cout << "Mode: " << Mode << endl;
	cout << "Dictionary File: " << Dictionary << endl << endl;


	/*=======================================================================================
     * Here we ewentually start our program according to recognized command line arguments
	 *=====================================================================================*/

	if (InFile != "" && OutFile != "" && Dictionary != "")
	{
		if (IfFileExists(InFile))
		{
			if (!empty(InFile))
			{
				if (Mode == "COMPRESS")//If the mode equals "Compress"
				{
					compress(InFile, OutFile, Dictionary);//Run function Compress
				}
				else if (Mode == "DECOMPRESS")//If the mode equals "Decompress"
				{
					decompress(InFile, OutFile, Dictionary);//Run function Deompress
				}
				else
				{
					error();
					cout << endl << "Error: Mode is not specified" << endl;
					return 0; //Finish the program execution
				}
			}
			else
			{
				error();
				cout << endl << "Error: File is empty" << endl;
				return 0; //Finish the program execution
			}
		}
		else
		{
			error();
			cout << endl << "Error: Cannot read input file" << endl;
			return 0; //Finish the program execution
		}
	}
	else
	{
		error();
		return 0; //Finish the program execution
	}

	return 0;
}