/*
	utilities.h

	Utilities to support interface

	Author: Brooke Czerwinski
	Date:	2/21/2020
	Class:	CS202
	Program #3

*/

#include <iostream>
#include <cstring>

const int MAX_CHAR = 256;

using std::ostream;
using std::istream;

class String
{
public:
	// Default constructor
	String() : str(0), len(0) {};

	// Copy constructor
	String(const String& source);

	// Constructor using String pointer
	explicit String(const String* source);

	// Constructor using char pointer
	explicit String(const char* source);

	// Destructor
	~String();

	// Assignment operator
	String& operator = (const String& source);
	String& operator = (const char* source);

	// Input and output operators
	friend ostream& operator << (ostream& out, const String& op2);
	friend istream& operator >> (istream& in, String& op2);

	// + operators
	friend String operator + (const String& op1, const char* op2);
	friend String operator + (const char* op1, const String& op2);
	friend String operator + (const String& op1, const String& op2);

	// += operators
	String& operator += (const String& op1);
	String& operator += (const char* op2);

	// Relational/Equality Operators
	// If we implement one of these we need to implement the whole range of them.
	friend bool operator < (const String& op1, const char* op2);
	friend bool operator < (const char* op1, const String& op2);
	friend bool operator < (const String& op1, const String& op2);

	friend bool operator <= (const String& op1, const char* op2);
	friend bool operator <= (const char* op1, const String& op2);
	friend bool operator <= (const String& op1, const String& op2);

	friend bool operator > (const String& op1, const char* op2);
	friend bool operator > (const char* op1, const String& op2);
	friend bool operator > (const String& op1, const String& op2);

	friend bool operator >= (const String& op1, const char* op2);
	friend bool operator >= (const char* op1, const String& op2);
	friend bool operator >= (const String& op1, const String& op2);

	friend bool operator != (const String& op1, const char* op2);
	friend bool operator != (const char* op2, const String& op1);
	friend bool operator != (const String& op1, const String& op2);

	friend bool operator == (const String& op1, const char* op2);
	friend bool operator == (const char* op1, const String& op2);
	friend bool operator == (const String& op1, const String& op2);


	// [] operator
	// Must be overloaded as a member function.
	// lvalue because
	char& operator[](int) const;

private:
	char*	str;	// The cstring containing the string data
	int		len;	// The length of the string

};


// Input Utilities
// Int Input validation
/*
	INPUT:	- int - input from client - input variable provided from client
			- cstring - prompt - prompt be displayed upon bad entry of data
	OUTPUT:	NONE
*/
void takeInput(int& input, const String& prompt);

// Float Input validation
/*
	INPUT:	- float - input from client - input variable provided from client
			- cstring - prompt - prompt be displayed upon bad entry of data
	OUTPUT:	NONE
*/
void takeInput(float& input, const String& prompt);

// Char Input validation
/*
	INPUT:	- char - input from client - input variable provided from client
			- cstring - prompt - prompt be displayed upon bad entry of data
	OUTPUT:	NONE
*/
void takeInput(char& input, const String& prompt);

// Cstring Input validation
/*
	INPUT:	- cstring - input from client - input variable provided from client
			- cstring - prompt - prompt be displayed upon bad entry of data
	OUTPUT:	NONE
*/
void takeInput(char input[], const String& prompt);

// Error Handling: Check for cin error
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

// Check for quit input from cstring input. Must be exact.
/*
	INPUT:	- cstring - input - input from user
	OUTPUT:	bool - returns true if quit is detected or false if not
*/
bool quitCheck(const char input[]);

// Check for quit input from char input
/*
	INPUT:	- char - input - input from user
	OUTPUT:	bool - returns true if quit is detected or false if not
*/
bool quitCheck(const char& input);

// Uses stringcompare but is case-insensitive
/*
	INPUT:	- char* - op1 - left operand in compare statement
			- char* - op2 - right operand in compare statement
	OUTPUT:	- int - returns the value of a lowercase strcmp
*/
int strcmpCI(const char* op1, const char* op2);

// Changes argument cstring to all lowercase
/*
	INPUT:	- char* - source - the cstring to change to lowercase
	OUTPUT:	- int - returns 1 for success and 0 for failure
*/
int toLowercase(char* source);
