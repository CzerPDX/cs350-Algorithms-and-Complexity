/*
	utilities.h

	Utilities to support interface

	Author: Brooke Czerwinski
	Date:	2/6/2020
	Class:	CS202
	Program #2

*/


#include <iostream>
#include <cstring>

const int MAX_CHAR = 256;

// Input Utilities
//
// Int Input validation
/*
	INPUT:	- int - input from client - input variable provided from client
		- cstring - prompt - prompt be displayed upon bad entry of data

	OUTPUT:	NONE
*/
void takeInput(int& input, const char prompt[]);



// Float Input validation
/*
	INPUT:	- float - input from client - input variable provided from client
		- cstring - prompt - prompt be displayed upon bad entry of data

	OUTPUT:	NONE
*/
void takeInput(float& input, const char prompt[]);



// Char Input validation
/*
	INPUT:	- char - input from client - input variable provided from client
		- cstring - prompt - prompt be displayed upon bad entry of data

	OUTPUT:	NONE
*/
void takeInput(char& input, const char prompt[]);



// Cstring Input validation
/*
	INPUT:	- cstring - input from client - input variable provided from client
		- cstring - prompt - prompt be displayed upon bad entry of data

	OUTPUT:	NONE
*/
void takeInput(char input[], const char prompt[]);



// Error Handling
//
// Check for cin error
/*
	INPUT:	- bool - errorFlag - tracks error status

	OUTPUT:	NONE
*/
void cinErrorCheck(bool& errorFlag);



// Check what is in the next space in input
/*
	INPUT:	- bool - errorFlag - tracks error status

	OUTPUT:	NONE
*/
void checkNext(bool& errorFlag);



// Invalid Entry output
/*
	INPUT:	- bool - errorFlag - tracks error status

	OUTPUT:	NONE
*/
void invalidEntry(const bool& errorFlag);



// Check for quit input from cstring input
// Must be exact.
/*
	INPUT:	- cstring - input - input from user

	OUTPUT:	NONE
*/
bool quitCheck(const char input[]);



// Check for quit input from char input
/*
	INPUT:	- char - input - input from user

	OUTPUT:	NONE
*/
bool quitCheck(const char& input);
