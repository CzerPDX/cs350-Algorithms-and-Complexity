/*

	item.h

	Manage the Item class and its derived classes

	Author: Brooke Czerwinski
	Date:	2/3/2021
	Class:	CS 202
	Program #2

*/

#include "utilities.h"

class Item {
public:
	// Default constructor
	Item();
	// Destructor
	virtual ~Item();

	// Never meant to be called, but for derived functions,
	// interface will be a function that
	// takes input from the user to set a class' private data members
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	virtual int interface() = 0; 	// Pure virtual function

	// Never meant to be called, but for derived functions, display will be a function
	// that outputs all of the private data members of the class
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	virtual int display() const = 0;	// Pure virtual function


	// Never meant to be called, but for derived functions,
	// Will release all dynamic memory and set all non-dynamic values to 0 or equivalent
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	virtual int clearValues() = 0;		// Pure virtual function
};


class GroceryItem : public Item {
public:
	// Default constructor
	GroceryItem();
	// Copy constructor
	GroceryItem(const GroceryItem& source);
	// Secondary Constructor
	GroceryItem(const char* name, const int& qty);
	// Destructor
	~GroceryItem();

	// Will give the interface to take grocery item information
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int interface();

	// Will output the private data members
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int display() const;

protected:
	// Assists in copying of the GroceryItem.
	// Only used by the GroceryList class
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int copy(GroceryItem& destination);

private:
	char*	name_;	// name of the grocery item
	int		qty_;	// quantity of the grocery item

	// Will release all dynamic memory and set all non-dynamic values to 0 or equivalent
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int clearValues();
};


class GroceryNode : public GroceryItem {
public:
	// Default constructor
	GroceryNode();
	// Copy constructor
	GroceryNode(const GroceryNode& source);
	// Secondary constructor. Implicit type conversion not allowed
	explicit GroceryNode(const GroceryItem& source);
	// Destructor
	~GroceryNode();

	// Returns a pointer to a GroceryNode object
	/*
		INPUT:	NONE
		OUTPUT:	- returns GroceryNode pointer to object's next
	*/
	GroceryNode*& getNext();

	// Sets the object's next pointer to the arguemtn
	/*
		INPUT:	- GroceryNode Object pointer - will become new next for the object
		OUTPUT:	- NONE
	*/
	void setNext(GroceryNode*& newNext);

	// Copies the calling details into the argument item
	/*
		INPUT:	- GroceryItem& - destination - 
		OUTPUT:	- NONE
	*/
	int copyWrapper(GroceryItem& destination);

private:
	GroceryNode*	next_;	// Pointer to the next GroceryNode

	// Will release all dynamic memory and set all non-dynamic values to 0 or equivalent
	/*
		INPUT:	- NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int clearValues();
};


class GroceryList : public Item {
public:
	// Default constructor
	GroceryList();
	// Copy constructor
	GroceryList(const GroceryList& source);
	// Secondary constructor. Implicit type conversion not allowed
	explicit GroceryList(const GroceryNode* source);
	// Destructor
	~GroceryList();

	// Provides the interface to interface a new grocery item to the GroceryList
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int interface();	

	// Calls recursive function to display grocery list
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int display() const;

	// Returns the size of the grocery List.
	// Only used by the day class LLL for compressed display
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int getSize() const;

	// Adds a GroceryItem to the GroceryList
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int add(const GroceryItem& newItem);

private:
	GroceryNode*	head_;	// head of the grocery LLL
	int				size_;

	// Will release all dynamic memory
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int clearValues();

	// Will release all dynamic memory recursively
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int clearValues(GroceryNode*& head);

	// Displays grocery list recursively
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int display(GroceryNode* head) const;

	// Calls recursive function to display a numbered grocery list
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int displayNumbered() const;

	// Displays a numbered grocery list recursively
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int display(GroceryNode* head, int& currentEntry) const;

	// Will provide the interface to remove a grocery item from the list
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int add();

	// Will provide the recursive function to add an item to the list
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int add(GroceryNode*& head, const GroceryItem& newItem);

	// Will provide the interface to remove a grocery item from the list
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int remove(); 	

	// Will provide the recursive function to remove an item from the list
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int remove(GroceryNode*& head, const int& entryToRemove, int& currentEntry, GroceryItem& returnedItem); 	
};


class School : public Item {
public:
	// Default constructor
	School();
	// Copy constructor
	School(const School& source);
	// Secondary constructor
	School(const char className[], const char homework[], const char notes[]);
	// Destructor
	~School();

	// Will give the interface to take school item info
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int interface();

	// Will display the private data members
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int display() const;

	// Will display the private data members
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int displayHomework() const;

private: 
	char*	className_;	// Name of the class for schoolwork
	char*	homework_;	// Details of the schoolwork
	char*	notes_;		// Any interfaceitional notes for the schoolwork

	// Will release all dynamic memory and set all non-dynamic values to 0 or equivalent
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int clearValues();
};


class Appointment : public Item {
public:
	// Default constructor
	Appointment();
	// Copy constructor
	Appointment(const Appointment& source);
	// Secondary constructor
	Appointment(const char time[], const char appointment[], const char notes[]);
	// Destructor
	~Appointment();


	// Will give the interface to take appointment item info
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int interface();	

	// Will display the private data members
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int display() const;

	// Will display the Appointment
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int displayAppointment() const;

	// Will display the time
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int displayTime() const;

private:
	char*	time_;			// Time of the appointment
	char*	appointment_;	// Appointment description
	char*	notes_;			// Notes for the appointment

	// Will release all dynamic memory and set all non-dynamic values to 0 or equivalent
	/*
		INPUT:	NONE
		OUTPUT:	- returns int - 0 for failure, 1 for success
	*/
	int clearValues();
};
