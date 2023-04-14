/*
        utilities.cpp

        Utilities to support interface

        Author: Brooke Czerwinski
        Date:	3/1/2020
        Class:	CS202
        Program #3

*/
#include "utilities.h"


using::std::cout;
using::std::endl;
using::std::cin;


String::String(const String& source) : str(nullptr), len(source.len) {
	if (source.str != nullptr) {
		str = new char[strlen(source.str) + 1];
		strcpy(str, source.str);
	}
}

String::String(const char* source) : str(nullptr), len(0) {
	if (source != nullptr) {
		len =  strlen(source);

		str = new char[len + 1];
		strcpy(str, source);
	}
}

String::String(const String* source) : str(nullptr), len(source->len) {
	if (source != nullptr) {
		str = new char[len + 1];
		strcpy(str, source->str);
	}
}

String::~String() {
	if (str != nullptr) {
		delete[] str;
		str = nullptr;
	}
	len = 0;
}

// Can't be const. Returning by reference because it's an lvalue
String& String::operator = (const String& source) {
	// Check for self assignment
	if (this != &source) {
		// If we already have value in our str delete it
		if (str != nullptr) {
			delete [] str;
			str = nullptr;
		}
		// Allocate space for a new char of the proper length
		str = new char[source.len + 1];
		// Copy the new str value into the current object
		strcpy(str, source.str);
		// Copy the len value into the current object
		len = source.len;
	}

	return *this;
}

// Returning by reference because it's an lvalue
String& String::operator = (const char* source) {
	if (source != nullptr) {
		int length = strlen(source);
		// If we already have value in our str delete it
		if (str != nullptr) {
			delete [] str;
			str = nullptr;
		}
		// Allocate space for a new char of the proper length
		str = new char[length + 1];
		// Copy the new str value into the current object
		strcpy(str, source);
		// Copy the len value into the current object
		len = length;
	}

	return *this;
}

// Input Stream
istream& operator >> (istream& in, String& aString) {
	char temp[MAX_CHAR];
	// Many people do not provide this operator:
	// We do not use prompts here because operators don't do that
	// We do not use ignore here because cin doesn't usually ignore

	in.get(temp, MAX_CHAR, '\n');	// Or should this be in.get?
	aString.len = strlen(temp);
	// If we already have value in our str delete it
	if (aString.str != nullptr) {
		delete [] aString.str;
		aString.str = nullptr;
	}

	// Copy over the new string
	aString.str = new char[aString.len + 1];
	strcpy(aString.str, temp);

	return in;	
}

// Output stream
ostream& operator << (ostream& out, const String& aString) {
	out << aString.str;
	return out;
}

// + Operator overloading
String operator + (const String& op1, const char* op2) {
	char* temp = new char[op1.len + strlen(op2) + 1];
	strcpy(temp, op1.str);
	strcat(temp, op2);

	return String(temp);
}

String operator + (const char* op1, const String& op2) {
	char* temp = new char[op2.len + strlen(op1) + 1];
	strcpy(temp, op2.str);
	strcat(temp, op1);

	return String(temp);
}

String operator + (const String& op1, const String& op2) {
	char* temp = new char[op1.len + op2.len + 1];
	strcpy(temp, op1.str);
	strcat(temp, op2.str);

	return String(temp);
}

String& String::operator += (const String& op2) {
	if (op2.str != nullptr) {
		len += op2.len;
		char* temp = new char[len + 1];

		if (str != nullptr) {
			// Copy the current object's string into temp
			strcpy(temp, str);
			// Copy the 2nd operands string into temp
			strcat(temp, op2.str);
		}
		else {
			strcpy(temp, op2.str);
		}

		// Copies over the pointer to str using the
		// assignment operator overloading that we wrote earlier
		if (str != nullptr) {
			delete[] str;
			str = nullptr;
		}	
		str = temp;
	}

	// We are just copying an address
	return *this;
}

String& String::operator += (const char* op2) {
	if (op2 != nullptr) {
		len += strlen(op2);
		char* temp = new char[len + 1];

		if (str != nullptr) {
			strcpy(temp, str);
			strcat(temp, op2);
		}
		else {
			strcpy(temp, op2);
		}

		if (str != nullptr) {
			delete[] str;
			str = nullptr;
		}
		str = temp;
	}

	return *this;
}

// < Operator Overloading
bool operator < (const String& op1, const char* op2) {
	return (strcmpCI(op1.str, op2) < 0);
}
bool operator < (const char* op1, const String& op2) {
	return (strcmpCI(op1, op2.str) < 0);
}
bool operator < (const String& op1, const String& op2) {
	return (strcmpCI(op1.str, op2.str) < 0);
}

// <= Operator Overloading
bool operator <= (const String& op1, const char* op2) {
	return (strcmpCI(op1.str, op2) <= 0);
}
bool operator <= (const char* op1, const String& op2) {
	return (strcmpCI(op1, op2.str) <= 0);
}
bool operator <= (const String& op1, const String& op2) {
	return (strcmpCI(op1.str, op2.str) <= 0);
}

// > Operator Overloading
bool operator > (const String& op1, const char* op2) {
	return (strcmpCI(op1.str, op2) > 0);
}
bool operator > (const char* op1, const String& op2) {
	return (strcmpCI(op1, op2.str) > 0);
}
bool operator > (const String& op1, const String& op2) {
	return (strcmpCI(op1.str, op2.str) > 0);
}

// >= Operator Overloading
bool operator >= (const String& op1, const char* op2) {
	return (strcmpCI(op1.str, op2) >= 0);
}
bool operator >= (const char* op1, const String& op2) {
	return (strcmpCI(op1, op2.str) >= 0);
}
bool operator >= (const String& op1, const String& op2) {
	return (strcmpCI(op1.str, op2.str) >= 0);
}

// != Operator Overloading
bool operator != (const String& op1, const char* op2) {
	return (strcmpCI(op1.str, op2) != 0);
}
bool operator != (const char* op1, const String& op2) {
	return (strcmpCI(op1, op2.str) != 0);
}
bool operator != (const String& op1, const String& op2) {
	return (strcmpCI(op1.str, op2.str) != 0);
}

// == Operator Overloading
bool operator == (const String& op1, const char* op2) {
	return (strcmpCI(op1.str, op2) == 0);
}
bool operator == (const char* op1, const String& op2) {
	return (strcmpCI(op1, op2.str) == 0);
}
bool operator == (const String& op1, const String& op2) {
	return (strcmpCI(op1.str, op2.str) == 0);
}

// [] Operator Overloading
char& String::operator[](int index) const {
	return str[index];
}


// Input Utilities
// Take an int input
void takeInput(int& input, const String& prompt) {
	bool errorFlag;
	errorFlag = false;
	do {
		errorFlag = false;

		cout << prompt;
		cin >> input;
		checkNext(errorFlag);
		cinErrorCheck(errorFlag);
	} while (errorFlag == true);
}

// Take a float input
void takeInput(float& input, const String& prompt) {
	bool errorFlag;
	errorFlag = false;
	do {
		errorFlag = false;

		cout << prompt;
		cin >> input;
		checkNext(errorFlag);
		cinErrorCheck(errorFlag);
	} while (errorFlag == true);
}

// Take a char input
void takeInput(char& input, const String& prompt) {
	bool errorFlag;

	do {
		errorFlag = false;

		cout << prompt;
		cin >> input;
		checkNext(errorFlag);
		cinErrorCheck(errorFlag);
	} while (errorFlag == true);
}

// Take a cstring input
void takeInput(char input[], const String& prompt) {
	bool errorFlag;
	errorFlag = false;

	do {
		errorFlag = false;

		cout << prompt;
		cin.get(input, MAX_CHAR, '\n');
		cinErrorCheck(errorFlag);
	} while (errorFlag == true);
}


// Error Utilities
// Check if cin is in error
void cinErrorCheck(bool& errorFlag) {
	if (errorFlag != true) {
		if (!cin) {
			cin.clear();
			errorFlag = true;
			invalidEntry(errorFlag);
		}
		else {
			cin.ignore(MAX_CHAR, '\n');
			errorFlag = false;
		}
	}
}

// Check to see if there is a nonspace character sitting in the input stream
// Sets errorFlag to true if a nonspace character is in the input stream
void checkNext(bool& errorFlag) {
	char checkNext;
	checkNext = cin.peek();

	// If there is not a space, mark errorFlag as true
	if (errorFlag != true) {
		if (!isspace(checkNext)) {
			cin.clear();
			errorFlag = true;
			invalidEntry(errorFlag);
		}
	}
}

// Invalid Entry
// Used to clear the input stream and echo error message.
// (cin.clear is not included so that it can be used in the more
// specific error handling function cinErrorCheck from above)
void invalidEntry(const bool& errorFlag) {
	if (errorFlag == true) {
		cin.ignore(MAX_CHAR, '\n');
		cout << endl;
		cout << "Invalid Input. Please try again." << endl;
	}
}

// Check for quit in user entry
// Check for entry of 'q' for quit for cstring input.
bool quitCheck(const char input[]) {
	if (((input[0] == 'q') || (input[0] == 'Q')) && (input[1] == '\0')) {
		return true;
	}
	else {
		return false;
	}
}

// Check for entry of 'q' for quit for char input
bool quitCheck(const char& input) {
	if ((input == 'q') || (input == 'Q')) {
		return true;
	}
	else {
		return false;
	}
}

// Compare String class objects
int strcmpCI(const char* op1, const char* op2) {
	int comparisonValue = 0;

	char* op1Lowercase = new char[strlen(op1) + 1];
	strcpy(op1Lowercase, op1);
	toLowercase(op1Lowercase);

	char* op2Lowercase = new char[strlen(op2) + 1];
	strcpy(op2Lowercase, op2);
	toLowercase(op2Lowercase);

	comparisonValue = strcmp(op1Lowercase, op2Lowercase);

	delete[] op1Lowercase;
	op1Lowercase = nullptr;
	delete[] op2Lowercase;
	op2Lowercase = nullptr;

	return comparisonValue;
}

int toLowercase(char* source) {
	int 	i;
	int	successValue = 0;
	int	length = 0;

	if (source != nullptr) {
		if (source != nullptr) {
			length = strlen(source);
		}

		for (i = 0; i < length; ++i) {
			source[i] = tolower(source[i]);
		}

		successValue = 1;
	}

	return successValue;
}
