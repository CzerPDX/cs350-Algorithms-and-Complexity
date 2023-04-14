/*

	calendar.h

	Manage the Calendar class (a DLL of days)

	Builds a DLL of days based on the users's input of what month/year it is

	Author: Brooke Czerwinski
	Date: 2/3/2021
	Class: CS 202
	Program #2

*/

#include "day.h"
#include <time.h>	// I hope it's ok that I included this! I use it to get the current year and month
#include <fstream>
#include <iomanip>

using std::ifstream;


// A DLL of days
class Calendar
{
public:
	// Default constructor
	Calendar();
	// Copy constructor
	Calendar(const Calendar& source);
	// Destructor
	~Calendar();

	// Displays the menu. Gives user an option to:
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
		Options in user menu:
		- add a day to the calendar

		Displays all of the days of the month in a list and whether there are
		any items on that day:
		=======================
		February
			1  - 0 item(s)
			2  - 1 item(s)
			3  - 5 item(s)
			...
			28 - 3 item(s)

		Then gives the user the options:

		1.  Look at, add, or remove a day's details
		2.  Go to the next month
		3.  Go to the previous month
		q.  Quit
	*/
	int interface();

private:
	Day*	head_;		 // Head of the DLL of days
	int	currentMonth_;	 // Current month being displayed
	int	daysInMonth_;	 // Number of days in the current month
	char*	monthName_;	 // Char array of the name of the current month
	int	currentYear_;	 // Current year being displayed

	// Sets the month, initializes the calendar, and then loads data from savedData.txt
	/*
		INPUT:	- int - month - month to set the as the calendar's current month
				- int - year - year to set as the calendar's current year
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int setMonth(const int& month, const int& year);

	// Displays all days in the month and the number of items they have in a list
	/*
		INPUT:	NONE
		OUTPUT:	returns int - 0 for failure, 1 for success
	*/
	int display() const;

	// Recursively Displays all days in the month and the number of items they have in a list
	/*
		INPUT:	- Day object pointer - current day node in the DLL of days
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int display(Day* head) const;

	// Recursively copies all days in the month and the number of items they have in a list
	/*
		INPUT:	- Day object pointer - source - current source day node in the DLL of days
				- Day object pointer - destination - current destination to copy into
				- Day object pointer - prev - previous day node in the DLL of days
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int copy(Day* source, Day*& destination, Day*& prev);

	// Wrapper for recursive initialize month function
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int initializeMonth();

	// Recursively creates a DLL of empty days at the calendar's head.
	/*
		INPUT:	- Day object pointer - head - current source day node in the DLL of days
				- Day object pointer - prev - previous day node in the DLL of days
				- int - currentDay - the current day we are on in the calendar
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int initializeMonth(Day*& head, Day*& prev, int& currentDay);

	// Adds a new day object to the calendar
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int add(Day*& head, ifstream& in);

	// Load the relevant data from savedData.txt into the current month
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int loadMonth();

	// Recursive - Pulls the line of data out of savedData.txt for comparison
	/*
		INPUT:	- Day object pointer - head - current source day node in the DLL of days
				- ifstream - in - the current input stream from the file savedData.txt
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int loadMonth(Day*& head, ifstream& in);

	// Recursively compares the pulled lines from savedData.txt to the list
	// To see where they fit, and calls the add function when the right place is found
	/*
		INPUT:	- Day object pointer - head - current source day node in the DLL of days
				- ifstream - in - the current input stream from the file savedData.txt
				- int - incomingMonth - the month of the current line from the file
				- int - incomingDay - the day from the current line from the file
				- int - incomingYear - the year from the current line from the file
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int loadMonth(Day*& head,
			ifstream& in,
			const int& incomingMonth,
			const int& incomingDay,
			const int& incomingYear);

	// Load a homework item from the file
	/*
		INPUT:	- Day object pointer - head - current source day node in the DLL of days
				- ifstream - in - the current input stream from the file savedData.txt
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int loadHomework(Day*& head, ifstream& in);

	// Load a Appointment item from the file
	/*
		INPUT:	- Day object pointer - head - current source day node in the DLL of days
				- ifstream - in - the current input stream from the file savedData.txt
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int loadAppointment(Day*& head, ifstream& in);

	// Load a grocery item from the file
	/*
		INPUT:	- Day object pointer - head - current source day node in the DLL of days
				- ifstream - in - the current input stream from the file savedData.txt
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int loadGrocery(Day*& head, ifstream& in);

	// Sets the next month as the current month
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int nextMonth();

	// Sets the previous month as the current month
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int prevMonth();

	// Gives interface to go to a specific month and year
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int goToMonth();

	// Gives the interface to call the details of any of the day objects in the current month
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int dayDetails();

	// Recursively finds the requested day and activates either its interface (grocery list)
	// or displays the full details of the item (appointments and school items)
	/*
		INPUT:	- Day object pointer - head - current source day node in the DLL of days
				- int - goToEntry - the user's requested day
				- int - currentEntry - the current day we are on in the calendar	
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int dayDetails(Day*& head, const int& goToEntry, int& currentEntry);

	// Clear all days in the calendar and release all dynamic memory
	/*
		INPUT:	- Day object pointer - head - current source day node in the DLL of days
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int clearValues(Day*& head);
};


