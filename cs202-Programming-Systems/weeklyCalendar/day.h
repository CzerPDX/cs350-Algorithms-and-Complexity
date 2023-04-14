/*

	day.h

	Manage the Day class

	Author: Brooke Czerwinski
	Date: 2/3/2021
	Class: CS 202
	Program #2

*/

#include "item.h"
class ItemNode : public Item {
public:
	// Default constructor
	ItemNode();
	// Copy constructor
	ItemNode(const ItemNode& source);
	// Secondary constructors. Implicit type conversion not allowed
	explicit ItemNode(const Item* source);
	// Destructor
	~ItemNode();

	int interface();
	int display() const;

	// Returns a pointer to a ItemNode object
	/*
		INPUT:	NONE
		OUTPUT:	- returns ItemNode pointer to object's next
	*/
	ItemNode*& getNext();

	// Sets the object's next pointer to the arguemtn
	/*
		INPUT:	- ItemNode Object pointer - will become new next for the object
		OUTPUT:	- NONE
	*/
	void setNext(ItemNode* newNext);

	Item*& getData();
	// Sets the object's data pointer to the argument
	/*
		INPUT:	- Item pointer - will become the new object for the itemNode
		OUTPUT:	- NONE
	*/
	void setData(Item*& newData);

private:
	ItemNode*	next_;	// Pointer to the next ItemNode
	Item*		data_;	// Pointer to the data for the item node

	// Will release all dynamic memory and set all non-dynamic values to 0 or equivalent
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int clearValues();
	int copyData(const Item* source);
};


// Both a LLL of ItemNodes and a Node for a DLL of days (the Calendar class)
class Day {
public:
	// Default contructor
	Day();
	// Coypy constructor
	Day(const Day& source);
	// Secondary Constructor
	Day(const int& day, const int& month, const int& year);
	// Destructor
	~Day();

	// Displays the day menu and takes input
	/*
		INPUT:	- NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success

		First displays all of the items for the day and then gives the user options:
		1.  Add a schoolwork item
		2.  Add a grocery list
		3.  Add an appointment
		4.  View the details of a daily item
		5.  Remove an item
		6.  Return to month view
	*/
	int interface();

	// Displays all of the items for the day
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int display() const;

	// Add a new item to the list using downcasting
	/*
		INPUT:	- Item object - newItem - can point to a grocery, appointment, or school item
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int add(const Item* newItem);
	
	// Add a new grocery item to the list
	// there is only one grocery list allowed per day so all groceries are added and removed from the same list
	/*
		INPUT:	- GroceryItem - newItem - new grocery to add to the list
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int addGrocery(const GroceryItem& newItem);

	// Returns the number of items in the list
	// Day is essentially a node and is being managed by the calendar
	/*
		INPUT:	NONE
		OUTPUT:	- returns the number of items in the day's item list
	*/
	int getNumberOfItems() const;

	// Returns the day for use in the calendar class DLL
	/*
		INPUT:	- None
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int getDay() const;

	// Returns the month for use in the calendar class DLL
	/*
		INPUT:	- None
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int getMonth() const;

	// Returns the year for use in the calendar class DLL
	/*
		INPUT:	- None
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int getYear() const;

	// Returns a pointer to a day object for use in the calendar DLL
	/*
		INPUT:	NONE
		OUTPUT:	- returns Day pointer to object's next
	*/
	Day*& getNext();

	// Sets the object's next pointer to the argument
	/*
		INPUT:	- Day Object pointer - will become new next for the object
		OUTPUT:	- NONE
	*/
	void setNext(Day*& newNext);

	// Returns a pointer to a day object
	/*
		INPUT:	- NONE
		OUTPUT:	- Returns Day pointer to object's prev
	*/
	Day*& getPrev();

	// Sets the object's next pointer to the arguemtn
	/*
		INPUT:	- Day Object pointer - will become new prev for the object
		OUTPUT:	- NONE
	*/
	void setPrev(Day*& newPrev);

private:
	ItemNode*	head_;			// Head of the LLL of items
	int 		numberOfItems_; // Number of items in the itemNode list
	Day*		next_;			// pointer to the next day node
	Day*		prev_;			// pointer to the previous day node
	int			day_;			// day of the date
	int			month_;			// month of the date
	int			year_;			// year of the date

	bool		groceryListFlag_;	// keeps track of whether there is a grocery list

	// Will release dynamic memory and set values to 0
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int clearValues();

	// Will release all dynamic memory and set all non-dynamic values to 0 or equivalent
	/*
		INPUT:	head/current node of the LLL of daily items
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int clearValues(ItemNode*& head);

	/*
		INPUT:	head/current node of the LLL of daily items
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int display(ItemNode* head, int& count) const;

	// Interface to add a school object to the day's list
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int addSchool();

	// Interface to add a grocery object to the day's list
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int groceryList();

	// Recursively find the correct itemNode for the grocery list
	/*
		INPUT:	- ItemNode object - head - Item node that contains the grocery list
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int addToGroceryList(ItemNode*& head);

	// Recursively add grocery to the grocery list
	/*
		INPUT:	- ItemNode object - head - Item node that contains the grocery list
				- GroceryItem - newItem - new item to add to the grocery list
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int addGrocery(ItemNode*& head, const GroceryItem& newItem);

	// Interface to add an appointment object to the day's list
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int addAppointment();

	// Interface to view the details of a list item
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int viewDetails();

	// Interface to remove a daily item from the list
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int remove();

	// Recursively removes a daily item from the list
	/*
		INPUT:	- ItemNode object - head - Head of the list of daily items
				- int - entryToRemove - Entry number to remove
				- int - currentEntry - Current entry within the daily item list
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int remove(ItemNode*& head, const int& entryToRemove, int& currentEntry);

	// Recursively add a daily item to the list
	/*
		INPUT:	- ItemNode object - head - Head of the list of daily items
				- Item pointer - newItem - Either a grocery, appointment, or school item to add
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int add(ItemNode*& head, const Item* newItem);

	// Recursively goes to the entry entered
	/*
		INPUT:	- ItemNode object - head - Head of the list of daily items
				- int - destinationEntry - Entry number to go to
				- int - currentEntry - Current entry within the daily item list
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int goToEntry(ItemNode*& head, int& destinationEntry, int& currentEntry);
};
