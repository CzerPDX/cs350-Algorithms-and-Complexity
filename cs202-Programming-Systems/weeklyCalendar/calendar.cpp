#include "calendar.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::setw;
using std::left;


//////////////////////////////////////////////////
// Calendar class

// Default Constructor
Calendar::Calendar() : head_(nullptr), 
			currentMonth_(0), 
			daysInMonth_(0),
			monthName_(nullptr),
			currentYear_(0) {}

// Copy Constructor
Calendar::Calendar(const Calendar& source) : head_(nullptr), 
						currentMonth_(source.currentMonth_),
						daysInMonth_(source.daysInMonth_),
						monthName_(nullptr),
						currentYear_(source.currentYear_)
{
	copy(source.head_, head_, head_);


	if (source.monthName_ != nullptr)
	{
		monthName_ = new char[strlen(source.monthName_) + 1];
		strcpy(monthName_, source.monthName_);
	}
}


// Destructor
Calendar::~Calendar()
{
	clearValues(head_);

	currentMonth_ = 0;
	daysInMonth_ = 0;

	if (monthName_ != nullptr)
	{
		delete[] monthName_;
		monthName_ = nullptr;
	}

	currentYear_ = 0;
}



int Calendar::clearValues(Day*& head)
{
	int successValue = 0;

	if (head != nullptr)
	{
		clearValues(head->getNext());

		delete head;
		head = nullptr;
	}

	return successValue;
}



int Calendar::copy(Day* source, Day*& destination, Day*& prev)
{
	int successValue = 0;


	if (source != nullptr)
	{
		destination = new Day(*source);
		successValue = 1;

		if (source->getPrev() != nullptr)
		{
			destination->setPrev(prev);
		}

		if (source->getNext() != nullptr)
		{
			successValue = copy(source->getNext(), 
						destination->getNext(),
						destination);
		}

	}


	return successValue;
}


int Calendar::interface()
{

/*
	PLAN:

	First we will want to load the current month.
		- So we'll get the current month and year
		- then we'll update the date for the calendar's current month and year
		- then we'll load a LLL of the proper number of days from a file.
			- The load function will read from a file of day items.
				- Each item will start with the year and the month
				- Followed by the type of item it is
					(Grocery list, appointment, or homework)
				- and after that it will be formatted by the type of data
				  it contains
				- Any time we come across either:
					- a date stamp higher than the one we are on
					- or the end of the file
					- or the end of the month we are generating
				  we will want to start a new day item

	When we load a new month,
		- first the current month must be saved out to our file
		- then the new month must be loaded as above
		- then display the new month and the menu.
		
*/


	// First get the current month and year using time_t and localtime
	// 	I hope that it's ok that I used the time.h library. 
	// 	I read about it at the following links:
	// 		- http://www.cplusplus.com/reference/ctime/time_t/
	// 		- http://www.cplusplus.com/reference/ctime/localtime/

	int 	successValue = 0;
	char	inputBuffer;

	// Get current month and year for default display
	time_t rawTime = time(nullptr);
	struct tm *adjustedTime = localtime(&rawTime);
	int month = adjustedTime->tm_mon + 1;
	int year = adjustedTime->tm_year + 1900;


	// Message to the user because save function does not work!
	cout << endl;
	cout << "NOTE =============================================================";
	cout << endl;
	cout << "Saved data is not implemented in this program." << endl;
	cout << " If you add information to this month and then go to another month" << endl;
	cout << "any information not in savedData.txt file will disappear." << endl;
	cout << endl;
	cout << "The application currently reloads from the file contents" << endl;
	cout << "whenever you switch to a new month, and the ability to add new";
	cout << endl;
	cout << "items to savedData.txt has not been implemented" << endl;
	cout << endl;
	cout << "However, this shouldn't affect ts basic/required functionality!" << endl;
	cout << "=================================================================";
	cout << endl;


	// Then we set the month and year.
	setMonth(month, year);

	// Then we provide a menu with options
	do
	{
		cout << endl << endl;
		cout << "     Daily Calendar    " << endl;
		cout << " ======================" << endl;
		// then we load any data for our date range into our calendar

		// then we display the list
		display();



		// Give a menu and take a menu option
		takeInput(inputBuffer, "\nWhat would you like to do:\n\n1. Look at, add, or remove a day's details\n2. Go to the next month\n3. Go to the previous month\n4. Go to a certain month and year\nq. Quit\n\nEnter your choice: ");

		switch(inputBuffer)
		{
			case '1':
				dayDetails();
				break;
			case '2':
				nextMonth();
				break;
			case '3':
				prevMonth();
				break;
			case '4':
				goToMonth();
				break;
			case 'q':
				cout << "Exiting..." << endl;
		}
	} while(quitCheck(inputBuffer) != true);
	

	return successValue;
}




int Calendar::setMonth(const int& month, const int& year)
{
	int successValue = 0;
	


	// Only update the information if month is within acceptable bounds.
	// We will allow users to enter data for any year they want
	if ((month >= 1) && (month <= 12))
	{
		// Clear the dynamic memory in monthName_ before changing it
		if (monthName_ != nullptr)
		{
			delete[] monthName_;
			monthName_ = nullptr;
		}

		// Fills out private member details for the current month on the calendar

		currentMonth_ = month;
		currentYear_ = year;

		// The remaining details are filled out based on the argument month:

		// January
		if (month == 1)
		{
			daysInMonth_ = 31;

			monthName_ = new char[strlen("Januaray") + 1];
			strcpy(monthName_, "January");
		}



		// February
		else if (month == 2)
		{
			// Leap year calculation 
			// Algorithm from Microsoft:
			// https://docs.microsoft.com/en-us/office/troubleshoot/excel/determine-a-leap-year
			// If the year is divisible by 4 it might be a leap year
			if ((currentYear_ % 4) == 0)
			{
				// If the year is also divisible by 100 it might be a leap year
				if ((currentYear_ % 100) == 0)
				{
					// If the year is divisible by 400 it is a leap year
					if ((currentYear_ % 400) == 0)
					{
						daysInMonth_ = 29;
					}
					// Otherwise, it is not a leap year
					else
					{
						daysInMonth_ = 28;
					}
				}
				// Otherwise it is definitely a leap year
				else
				{
					daysInMonth_ = 29;
				}
			}
			// Otherwise not a leap year
			else
			{
				daysInMonth_ = 28;
			}

			monthName_ = new char[strlen("February") + 1];
			strcpy(monthName_, "February");
		}



		// March
		else if (month == 3)
		{
			daysInMonth_ = 31;

			monthName_ = new char[strlen("March") + 1];
			strcpy(monthName_, "March");
		}



		// April
		else if (month == 4)
		{
			daysInMonth_ = 30;

			monthName_ = new char[strlen("April") + 1];
			strcpy(monthName_, "April");
		}



		// May
		else if (month == 5)
		{
			daysInMonth_ = 31;

			monthName_ = new char[strlen("May") + 1];
			strcpy(monthName_, "May");
		}



		// June
		else if (month == 6)
		{
			daysInMonth_ = 30;

			monthName_ = new char[strlen("June") + 1];
			strcpy(monthName_, "June");
		}



		// July
		else if (month == 7)
		{
			daysInMonth_ = 31;

			monthName_ = new char[strlen("July") + 1];
			strcpy(monthName_, "July");
		}



		// August
		else if (month == 8)
		{
			daysInMonth_ = 31;

			monthName_ = new char[strlen("August") + 1];
			strcpy(monthName_, "August");
		}



		// September
		else if (month == 9)
		{
			daysInMonth_ = 30;

			monthName_ = new char[strlen("September") + 1];
			strcpy(monthName_, "September");
		}



		// October
		else if (month == 10)
		{
			daysInMonth_ = 31;

			monthName_ = new char[strlen("October") + 1];
			strcpy(monthName_, "October");
		}



		// November
		else if (month == 11)
		{
			daysInMonth_ = 31;

			monthName_ = new char[strlen("November") + 1];
			strcpy(monthName_, "November");
		}


		// December
		else if (month == 12)
		{
			daysInMonth_ = 31;

			monthName_ = new char[strlen("December") + 1];
			strcpy(monthName_, "December");
		}

		successValue = initializeMonth();

	}

	return successValue;
}


int Calendar::initializeMonth()
{
	int successValue = 0;

	if (head_ != nullptr)
	{
		clearValues(head_);
	}

	int startDay = 1;

	successValue = initializeMonth(head_, head_, startDay);

	// Load the values for the new month
	successValue = loadMonth();

	return successValue;
}


int Calendar::initializeMonth(Day*& head, Day*& prev, int& currentDay)
{
	int successValue = 0;

	if ((head == nullptr) && (currentDay <= daysInMonth_))
	{
		head = new Day(currentDay, currentMonth_, currentYear_);

		// Set the previous ptr as long as we aren't the head of the list
		// prev_ will stay nullptr for the head of the list.
		if (head != head_)
		{
			head->setPrev(prev);
		}

		successValue = initializeMonth(head->getNext(), head, ++currentDay);
		successValue = 1;
	}

	return successValue;
}


int Calendar::add(Day*& head, ifstream& in)
{
	int 	successValue = 0;
	char	typeBuffer[MAX_CHAR];

	if (head != nullptr)
	{
		// Find out what type of daily item we're adding
		in.get(typeBuffer, MAX_CHAR, ';');

		// Add a grocery list
		if (strcmp("Grocery", typeBuffer) == 0)
		{
			successValue = loadGrocery(head, in);
		}
		// Add an appointment
		else if (strcmp("Appointment", typeBuffer) == 0)
		{
			successValue = loadAppointment(head, in);
		}
		// Add a homework item
		else if (strcmp("School", typeBuffer) == 0)
		{
			successValue = loadHomework(head, in);
		}
	}

	return successValue;
}



/*
	- The load function will read from a file of day items.
		- Each item will start with the year and the month
			 - year;month;
		- Followed by the type of item it is
			(Grocery list, appointment, or homework)
		- and after that it will be formatted by the type of data
		  it contains
		- Any time we come across either:
			- a date stamp higher than the one we are on
			- or the end of the file
			- or the end of the month we are generating
		  we will want to start a new day item
*/


int Calendar::loadMonth()
{
	int 		successValue = 0;
	char 		fileName[] = "savedData.txt";
	ifstream 	in;

	
	// First open the file
	in.open(fileName);

	// If the file fails to open
	if (!in)
	{
		cout << endl << "Failed to open " << fileName << "." << endl;
	}
	// Otherwise, start processing it recursively
	else
	{
		successValue = loadMonth(head_, in);
	}

	
	return successValue;
}



int Calendar::loadMonth(Day*& head, ifstream& in)
{
	int 	successValue = 0;

	int 	incomingMonth;
	int 	incomingDay;
	int 	incomingYear;




	while ((!in.eof()) && (successValue != -1))
	{
		in >> incomingMonth;
		in.get();	// get rid of the ; delimiter
		in >> incomingDay;
		in.get();	// get rid of the ; delimiter
		in >> incomingYear;
		in.get();	// get rid of the ; delimiter

		// We want to ignore the incoming year if it is either prior to or
		// after our current year

		// If our incoming date is prior to our desired date
		if ((incomingYear <= currentYear_) && (incomingMonth < currentMonth_))
		{
			// Get rid of the rest of the line
			in.ignore(MAX_CHAR, '\n');
			//in.get();	// get rid of the \n

			if (in)
			{
			}
			else
			{
				successValue = -1;
			}
		}
		// If our incoming date is the same year and month as our current
		// We want to send it on further
		else if ((incomingYear == currentYear_) && 
				(incomingMonth == currentMonth_))
		{
			successValue = loadMonth(head, 
					in, 
					incomingMonth, 
					incomingDay, 
					incomingYear);
		}
		// Otherwise, if our incoming date is after the month and year of our 
		// current date, then we just want to stop trying bc there will not be
		// any more information for us.
		else
		{
			successValue = -1;
		}
	}

	return successValue;
}




int Calendar::loadMonth(Day*& head,
	ifstream& in,
	const int& incomingMonth,
	const int& incomingDay,
	const int& incomingYear)
{
	int 	successValue = 0;
	int 	currentDay = head->getDay();

	if ((head != nullptr) && (!in.eof()))
	{
		// If the date is equal to the current date
		// add the incoming item to the day
		if ((incomingYear == currentYear_) &&
			(incomingMonth == currentMonth_) &&
			(incomingDay == currentDay))
		{
			// Add it to the day
			successValue = add(head, in);
		}
		// Otherwise, if the incoming date is in the right month and year
		// and is less than the current date call the function again
		// but pass in the next node
		// The next node might be the right one
		else if ((incomingYear == currentYear_) &&
			(incomingMonth == currentMonth_) &&
			(currentDay < daysInMonth_))
		{
			successValue = loadMonth(head->getNext(),
				in,
				incomingMonth,
				incomingDay,
				incomingYear);
		}
	}

	return successValue;
}



int Calendar::loadHomework(Day*& destination, ifstream& in)
{
	int successValue = 0;

	if (destination != nullptr)
	{
		char		nameBuffer[MAX_CHAR];
		char		homeworkBuffer[MAX_CHAR];
		char		notesBuffer[MAX_CHAR];

		in.get();
		in.get(nameBuffer, MAX_CHAR, ';');
		in.get();	// get rid of the ; delimiter
		in.get(homeworkBuffer, MAX_CHAR, ';');
		in.get();	// get rid of the ; delimiter
		in.get(notesBuffer, MAX_CHAR, '\n');
		in.get();	// get rid of the \n delimiter

		School schoolBuffer(nameBuffer, homeworkBuffer, notesBuffer);

		Item* newItem = &schoolBuffer;
		successValue = destination->add(newItem);
	}

	return successValue;
}




int Calendar::loadAppointment(Day*& destination, ifstream& in)
{
	int successValue = 0;

	if (destination != nullptr)
	{
		char		timeBuffer[MAX_CHAR];
		char		appointmentInfo[MAX_CHAR];
		char		notesBuffer[MAX_CHAR];

		in.get();
		in.get(timeBuffer, MAX_CHAR, ';');
		in.get();	// get rid of the ; delimiter
		in.get(appointmentInfo, MAX_CHAR, ';');
		in.get();	// get rid of the ; delimiter
		in.get(notesBuffer, MAX_CHAR, '\n');
		in.get();	// get rid of the \n delimiter

		Appointment appBuffer(timeBuffer, appointmentInfo, notesBuffer);
		Item* newItem = &appBuffer;

		successValue = destination->add(newItem);
	}

	return successValue;
}




int Calendar::loadGrocery(Day*& destination, ifstream& in)
{
	int successValue = 0;

	if (destination != nullptr)
	{
		char		nameBuffer[MAX_CHAR];
		int		qtyBuffer;
		char		checkNext;

		in.get(checkNext);

		while (checkNext != '\n')
		{
			in.get(nameBuffer, MAX_CHAR, ';');
			in.get();
			in >> qtyBuffer;
			in.get(checkNext);


			GroceryItem groceryBuffer(nameBuffer, qtyBuffer);
			destination->addGrocery(groceryBuffer);
		}

		successValue = 1;
	}

	return successValue;
}



int Calendar::nextMonth()
{
	int successValue = 0;
	int month = currentMonth_;
	int year = currentYear_;

	if (month < 12)
	{
		successValue = setMonth(++month, year);
	}
	else
	{
		month = 1;
		successValue = setMonth(month, ++year);
	}

	return successValue;
}




int Calendar::prevMonth()
{
	int successValue = 0;
	int month = currentMonth_;
	int year = currentYear_;

	if (month > 1)
	{
		successValue = setMonth(--month, year);
	}
	else
	{
		month = 12;
		successValue = setMonth(month, --year);
	}

	return successValue;
}



int Calendar::goToMonth()
{
	int 		successValue = 0;
	int		yearBuffer = 0;
	int		monthBuffer = 0;

	cout << endl;
	cout << "Go to a specific month" << endl;
	cout << "======================" << endl << endl;

	
	while ((monthBuffer < 1) || (monthBuffer > 12))
	{
		takeInput(monthBuffer, "\nEnter the the month (use numbers): ");

		if ((monthBuffer < 1) || (monthBuffer > 12))
		{
			cout << "Invalid month. Please try again." << endl;
		}
	}
	takeInput(yearBuffer, "\nEnter the year: ");

	successValue = setMonth(monthBuffer, yearBuffer);

	
	

	return successValue;
}



int Calendar::dayDetails()
{
	int 		successValue = 0;
	int		inputBuffer = 0;

	cout << endl;
	display();
	cout << endl;

	takeInput(inputBuffer, "\nEnter the number of the day you want to go to: ");

	if ((inputBuffer >= 1) && (inputBuffer <= daysInMonth_))
	{
		int count = 1;
		successValue = dayDetails(head_, inputBuffer, count);

		if (successValue != 1)
		{
			cout << endl;
			cout << "Failed to interact with that item";
			cout << endl;
		}
	}
	else
	{
		cout << endl;
		cout << "Invalid day! That day does not exist in this month.";
		cout << endl;
	}


	return successValue;
}


int Calendar::dayDetails(Day*& head, const int& goToEntry, int& currentEntry)
{
	int successValue = 0;

	if (head != nullptr)
	{
		if (currentEntry < goToEntry)
		{
			++currentEntry;

			// We use lookahead here so that our successValue can be communicated
			if (head->getNext() != nullptr)
			{
				successValue = dayDetails(head->getNext(),
					goToEntry,
					currentEntry);
			}
		}
		else if (goToEntry == currentEntry)
		{
			successValue = head->interface();
		}
	}

	return successValue;
}
















int Calendar::display() const
{
	cout << "     " << monthName_ << " " << currentYear_ << endl;
	cout << " ======================" << endl;

	cout << setw(5) << " Day ";
	cout << " # items contained" << endl;
	cout << setw(5) << " === ";
	cout << " =================" << endl;
	return display(head_);
}


int Calendar::display(Day* head) const
{
	int successValue = 0;

	if (head != nullptr)
	{
		cout << " " << setw(5) << left << head->getDay() << " ";

		if (head->getNumberOfItems() > 0 )
		{
			cout << "  " << head->getNumberOfItems() << " item(s)";
		}
		else
		{
			cout << "      -";
		}

		cout << endl;

		successValue = display(head->getNext());
	}
	else
	{
		successValue = 1;
	}

	return successValue;
}
