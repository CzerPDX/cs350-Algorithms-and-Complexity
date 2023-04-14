/*
        utilities.cpp

        Utilities to support interface

        Author: Brooke Czerwinski
        Date:	2/6/2020
        Class:	CS202
        Program #2

*/

#include "utilities.h"


using::std::cout;
using::std::endl;
using::std::cin;

// Input Utilities
//
// Take an int input
void takeInput(int& input, const char prompt[])
{
        bool errorFlag;
        errorFlag = false;
        do
        {
                errorFlag = false;

                cout << prompt;
                cin >> input;
                checkNext(errorFlag);
                cinErrorCheck(errorFlag);
        } while (errorFlag == true);
}



// Take a float input
void takeInput(float& input, const char prompt[])
{
        bool errorFlag;
        errorFlag = false;
        do
        {
                errorFlag = false;

                cout << prompt;
                cin >> input;
                checkNext(errorFlag);
                cinErrorCheck(errorFlag);
        } while (errorFlag == true);
}



// Take a char input
void takeInput(char& input, const char prompt[])
{
        bool errorFlag;

        do
        {
                errorFlag = false;

                cout << prompt;
                cin >> input;
                checkNext(errorFlag);
                cinErrorCheck(errorFlag);
        } while (errorFlag == true);
}



// Take a cstring input
void takeInput(char input[], const char prompt[])
{
        bool errorFlag;
        errorFlag = false;

        do
        {
                errorFlag = false;

                cout << prompt;
                cin.get(input, MAX_CHAR, '\n');
                cinErrorCheck(errorFlag);
        } while (errorFlag == true);
}




// Error Utilities
//
// Check if cin is in error
void cinErrorCheck(bool& errorFlag)
{
        if (errorFlag != true)
        {
                if (!cin)
                {
                        cin.clear();
                        errorFlag = true;
                        invalidEntry(errorFlag);
                }
                else
                {
                        cin.ignore(MAX_CHAR, '\n');
                        errorFlag = false;
                }
        }
}



// Check to see if there is a nonspace character sitting in the input stream
// Sets errorFlag to true if a nonspace character is in the input stream
void checkNext(bool& errorFlag)
{
        char checkNext;
        checkNext = cin.peek();

        // If there is not a space, mark errorFlag as true
        //
        if (errorFlag != true)
        {
                if (!isspace(checkNext))
                {
                        cin.clear();
                        errorFlag = true;
                        invalidEntry(errorFlag);
                }
        }
}



// Invalid Entry
//
// Used to clear the input stream and echo error message.
// (cin.clear is not included so that it can be used in the more
// specific error handling function cinErrorCheck from above)
void invalidEntry(const bool& errorFlag)
{
        if (errorFlag == true)
        {
                cin.ignore(MAX_CHAR, '\n');
                cout << endl;
                cout << "Invalid Input. Please try again." << endl;
        }
}



// CHECK FOR QUIT
//
// Check for entry of 'q' for quit for cstring input.
bool quitCheck(const char input[])
{
        if (((input[0] == 'q') || (input[0] == 'Q')) && (input[1] == '\0'))
        {
                return true;
        }
        else
        {
                return false;
        }
}



// Check for entry of 'q' for quit for char input
bool quitCheck(const char& input)
{
	if ((input == 'q') || (input == 'Q'))
        {
                return true;
        }
        else
        {
                return false;
        }
}

