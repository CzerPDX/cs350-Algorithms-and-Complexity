/*

        day.cpp

        Manage the Day class

        Author: Brooke Czerwinski
        Date: 	2/3/2021
        Class: 	CS 202
        Program #2

*/



#include "day.h"



using std::cout;
using std::endl;


//////////////////////////////////////////////////
// ItemNode class

// Default Constructor
ItemNode::ItemNode() : next_(nullptr), data_(nullptr) {}

// Copy constructor
ItemNode::ItemNode(const ItemNode& source) : next_(nullptr)
{
	// Copy the data info

	copyData(source.data_);

	// Recursively copy all other nodes
	if (source.next_ != nullptr)
	{
		next_ = new ItemNode(*source.next_);
	}
}


ItemNode::ItemNode(const Item* source) : next_(nullptr)
{
	copyData(source);
}







ItemNode::~ItemNode()
{
	clearValues();
}


int ItemNode::clearValues()
{
	int successValue = 0;

	if (data_ != nullptr)
	{
		delete data_;
		data_ = nullptr;
	}

	if (next_ != nullptr)
	{
		next_ = nullptr;
	}

	if ((data_ == nullptr) && (next_ == nullptr))
	{
		successValue = 1;
	}

	return successValue;
}

int ItemNode::copyData(const Item* source)
{
	int successValue = 0;

	if (source != nullptr)
	{
		// Try to save item as a ptr to each type of daily item.
		const GroceryList* groceryPtr = dynamic_cast<const GroceryList*>(source);

		if (groceryPtr != nullptr)
		{
			data_ = new GroceryList(*groceryPtr);
		}
		// Otherwise we need to try to do this with the other 2 types
		else
		{
			// Try to save the data as a school item
			const School* schoolPtr = dynamic_cast<const School*>(source);

			// If the ptr is not null the item was a school item
			if (schoolPtr != nullptr)
			{
				// So you can use the copy constructor.
				data_ = new School(*schoolPtr);
			}
			// Otherwise it must have been an appointment item
			else
			{
				const Appointment* appPtr = dynamic_cast<const Appointment*>(source);

				if (appPtr != nullptr)
				{
					data_ = new Appointment(*appPtr);
				}
			}
		}
	}

	return successValue;
}


int ItemNode::interface()
{
	int successValue = 0;

	if (data_ != nullptr)
	{
		data_->interface();
		successValue = 1;
	}

	return successValue;
}


int ItemNode::display() const
{
	int successValue = 0;

	if (data_ != nullptr)
	{
		data_->display();
		successValue = 1;
	}

	return successValue;
}


ItemNode*& ItemNode::getNext()
{
	return next_;
}


void ItemNode::setNext(ItemNode* newNext)
{
	next_ = newNext;
}



Item*& ItemNode::getData()
{
	return data_;
}


void ItemNode::setData(Item*& newData)
{
	// We want the node to manage its own dynamic memory so we will make a copy
	// of any incoming information

	if (newData != nullptr)
	{
		// Try to save item as a ptr to each type of daily item.
		GroceryList* groceryPtr = dynamic_cast<GroceryList*>(newData);

		if (groceryPtr != nullptr)
		{
			data_ = new GroceryList(*groceryPtr);
		}
		// Otherwise we need to try to do this with the other 2 types
		else
		{
			// Try to save the data as a school item
			School* schoolPtr = dynamic_cast<School*>(newData);

			// If the ptr is not null the item was a school item
			if (schoolPtr != nullptr)
			{
				// So you can use the copy constructor.
				data_ = new School(*schoolPtr);
			}
			// Otherwise it must have been an appointment item
			else
			{
				Appointment* appPtr = dynamic_cast<Appointment*>(newData);

				if (appPtr != nullptr)
				{
					data_ = new Appointment(*appPtr);
				}
			}
		}
	}
}


//////////////////////////////////////////////////
// Day class

// Default Constructor
Day::Day() : head_(nullptr), 
		numberOfItems_(0),
		next_(nullptr), 
		prev_(nullptr), 
		day_(0), 
		month_(0), 
		year_(0),
		groceryListFlag_(0) {}

// Copy constructor
Day::Day(const Day& source) : head_(nullptr), 
				numberOfItems_(0),
				next_(nullptr), 
				prev_(nullptr), 
				day_(source.day_), 
				month_(source.month_), 
				year_(source.year_),
				groceryListFlag_(source.groceryListFlag_)
{
	if (source.head_ != nullptr)
	{
		// This will recursively recreate the entire LLL
		head_ = new ItemNode(*source.head_);
	}
}


// Secondary constructor
Day::Day(const int& day, const int& month, const int& year) : head_(nullptr),
							numberOfItems_(0),
							next_(nullptr),
							prev_(nullptr),
							day_(day),
							month_(month),
							year_(year),
							groceryListFlag_(0) {}



Day::~Day()
{
	clearValues();
}


int Day::clearValues()
{
	int successValue = 0;

	clearValues(head_);

	if (next_ != nullptr)
	{
		next_ = nullptr;
	}

	if (prev_ != nullptr)
	{
		prev_ = nullptr;
	}

	day_ = 0;
	month_ = 0;
	year_ = 0;
	groceryListFlag_ = 0;

	if ((head_ == nullptr) && (prev_ == nullptr) && (prev_ == nullptr))
	{
		successValue = 1;
	}

	return successValue;
}




int Day::clearValues(ItemNode*& head)
{
	int successValue = 0;

	if (head != nullptr)
	{
		successValue = clearValues(head->getNext());

		delete head;
		head = nullptr;
		successValue = 1;
	}

	return successValue;
}


int Day::interface()
{
	int 	successValue = 0;
	char	inputBuffer;



	do
	{
		cout << endl << endl;
		cout << "Daily Item List for ";
		cout << month_ << "/" << day_ << "/" << year_ << endl;
		cout << "==============================" << endl;
		// Will display all of the items in the list
		display();
		cout << endl;


		// Give a menu and take a menu option
		takeInput(inputBuffer, "\nWhat would you like to do:\n\n1. Add a schoolwork item\n2. View or edit the grocery list\n3. Add an appointment\n4. View the details of a daily item\n5. Remove a daily item from the list \nq. Go back to month view\n\nEnter your choice: ");

		switch(inputBuffer)
		{
			case '1':
				addSchool();
				break;
			case '2':
				groceryList();
				break;
			case '3':
				addAppointment();
				break;
			case '4':
				viewDetails();
				break;
			case '5':
				remove();
				break;
			case 'q':
				cout << "Returning..." << endl;
		}

	} while(quitCheck(inputBuffer) != true);
	

	return successValue;
}



int Day::display() const
{
	int successValue = 0;

	if (head_ != nullptr)
	{
		// Count will number the entries
		int count = 1;
		return display(head_, count);
	}
	else
	{
		cout << "Today's item list is empty" << endl;
	}

	return successValue;
}




int Day::display(ItemNode* head, int& count) const
{
	int successValue = 0;


	if ((head != nullptr) && (head->getData() != nullptr))
	{
		// Number the list
		cout << count << ". ";

		// Try to save the head as a grocery list pointer
		GroceryList* groceryPtr = dynamic_cast<GroceryList*>(head->getData());

		// If the head is a grocery list item
		if (groceryPtr != nullptr)
		{
			cout << "Grocery List (";
			cout << groceryPtr->getSize();
			cout << " items)";
			successValue = 1;
		}
		// Otherwise it is either school or appointment
		else
		{
			// Try to save the head as a school item pointer
			School* schoolPtr = dynamic_cast<School*>(head->getData());

			// If the head is a school item
			if (schoolPtr != nullptr)
			{
				schoolPtr->displayHomework();
				successValue = 1;
			}
			// Otherwise it must have been an appointment item
			else
			{
				// Try to save the head as an appointment item pointer
				Appointment* appPtr = dynamic_cast<Appointment*>(head->getData());
				
				// If the head is an appointment item
				if (appPtr != nullptr)
				{
					appPtr->displayAppointment();
					cout << " at ";
					appPtr->displayTime();
					successValue = 1;
				}
			}
		}
		cout << endl;
		display(head->getNext(), ++count);
	}

	return successValue;
}




int Day::addSchool()
{
	int successValue = 0;
	School	schoolObj;

	successValue = schoolObj.interface();

	if (successValue != 0)
	{
		Item*	ptr = &schoolObj;
		successValue = add(head_, ptr);
	}
	else
	{
		cout << endl << "Failed to take item info" << endl;
	}

	return successValue;
}


int Day::addGrocery(const GroceryItem& newItem)
{
	return addGrocery(head_, newItem);
}


int Day::addGrocery(ItemNode*& head, const GroceryItem& newItem)
{
	int successValue = 0;


	// If there is no grocery list yet create onel
	if (groceryListFlag_ != true)
	{
		GroceryList	groceryListObj;

		successValue = groceryListObj.add(newItem);

		if (successValue != 0)
		{
			Item* ptr = &groceryListObj;
			successValue = add(head_, ptr);

			groceryListFlag_ = true;
		}
		else
		{
			cout << endl << "Failed to create grocery list" << endl;
		}
	}
	else if (head != nullptr)
	{
		GroceryList* groceryPtr = dynamic_cast<GroceryList*>(head->getData());

		// If we have located the grocery list, add the item
		if (groceryPtr != nullptr)
		{
			successValue = groceryPtr->add(newItem);

			successValue = 1;
		}
		// Otherwise keep searching recursively
		else
		{
			successValue = addGrocery(head->getNext(), newItem);
		}
	}

	return successValue;
}

int Day::groceryList()
{
	int successValue = 0;

	// If there is no grocery list yet create onel
	if (groceryListFlag_ != true)
	{
		GroceryList	groceryListObj;

		successValue = groceryListObj.interface();

		if (successValue != 0)
		{
			Item* ptr = &groceryListObj;
			successValue = add(head_, ptr);

			groceryListFlag_ = true;
		}
		else
		{
			cout << endl << "Failed to create grocery list" << endl;
		}
	}
	else
	{
		successValue = addToGroceryList(head_);
	}

	return successValue;
}

int Day::addToGroceryList(ItemNode*& head)
{
	int successValue = 0;

	if (head != nullptr)
	{
		GroceryList* groceryPtr = dynamic_cast<GroceryList*>(head->getData());

		if (groceryPtr != nullptr)
		{
			successValue = groceryPtr->interface();

			// Need to check if the grocery list is empty now.
			if (groceryPtr->getSize() == 0)
			{
				// Delete if the list is empty now.
				ItemNode* hold = head;
				head = head->getNext();

				delete hold;
				groceryListFlag_ = false;
			}

			successValue = 1;
		}
		else
		{
			successValue = addToGroceryList(head->getNext());
		}
	}

	return successValue;
}


int Day::addAppointment()
{
	int successValue = 0;
	Appointment	appointmentObj;

	successValue = appointmentObj.interface();

	if (successValue != 0)
	{
		Item*	ptr = &appointmentObj;
		successValue = add(head_, ptr);
	}
	else
	{
		cout << endl << "Failed to take item info" << endl;
	}

	return successValue;
}




int Day:: add(const Item* newItem)
{
	return add(head_, newItem);
}



int Day::add(ItemNode*& head, const Item* newItem)
{
	int successValue = 0;
	
	if (head != nullptr)
	{
		successValue = add(head->getNext(), newItem);
	}
	else
	{
		head = new ItemNode(newItem);
		++numberOfItems_;

		successValue = 1;
	}

	return successValue;
}


int Day::getNumberOfItems() const
{
	return numberOfItems_;
}



int Day::getDay() const
{
	return day_;
}

int Day::getMonth() const
{
	return month_;
}

int Day::getYear() const
{
	return year_;
}



Day*& Day::getNext()
{
	return next_;
}



void Day::setNext(Day*& newNext)
{
	next_ = newNext;
}


Day*& Day::getPrev()
{
	return prev_;
}

void Day::setPrev(Day*& newPrev)
{
	prev_ = newPrev;
}




int Day::viewDetails()
{
	int 		successValue = 0;
	GroceryItem	returnedItem;
	int		inputBuffer = 0;

	successValue = display();

	if (successValue != 0)
	{
		cout << endl << endl;
		cout << "Daily Item List" << endl;
		cout << "===============" << endl;
		// Will display all of the items in the list
		display();
		cout << endl;

		takeInput(inputBuffer, "\nEnter the number of the daily item to interact with: ");

	
		if (inputBuffer <= numberOfItems_)
		{
			int count = 1;
			successValue = goToEntry(head_, inputBuffer, count);

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
			cout << "There aren't that many items on your daily item list!" << endl;
		}

	}
	

	return successValue;
}



// Activates the interface of grocery list or display function of items at the
// nth ItemNode in the item list
int Day::goToEntry(ItemNode*& head, int& destinationEntry, int& currentEntry)
{
	int successValue = 0;

	if (head != nullptr)
	{
		if (currentEntry < destinationEntry)
		{
			++currentEntry;

			// We use lookahead here so that our successValue can be communicated
			if (head->getNext() != nullptr)
			{
				successValue = goToEntry(head->getNext(),
							destinationEntry,
							currentEntry);
			}
		}
		else if (destinationEntry == currentEntry)
		{
			// try to cast it to a grocery List pointer
			GroceryList* groceryPtr = dynamic_cast<GroceryList*>(head->getData());
			// If it is a grocery list, open the interface
			if (groceryPtr != nullptr)
			{
				successValue = addToGroceryList(head);
			}
			else
			{
				// If it is a school item display the details
				
				School* schoolPtr = dynamic_cast<School*>(head->getData());	
			
				if (schoolPtr != nullptr)
				{
					cout << endl;
					cout << "School homework details" << endl;
					cout << "=======================" << endl;
					schoolPtr->display();
				}
				else
				{

					Appointment* appPtr = dynamic_cast<Appointment*>(head->getData());	

					if (appPtr != nullptr)
					{
						cout << endl;
						cout << "Appointment details" << endl;
						cout << "===================" << endl;
						appPtr->display();
					}
				}
			}
			// If it is an appointment, display the details

			successValue = 1;
		}
	}

	return successValue;
}





int Day::remove()
{
	int 		successValue = 0;
	GroceryItem	returnedItem;
	int		inputBuffer = 0;

	successValue = display();

	if (successValue != 0)
	{
		cout << endl << endl;
		cout << "Daily Item List" << endl;
		cout << "===============" << endl;
		// Will display all of the items in the list
		display();
		cout << endl;

		takeInput(inputBuffer, "\nEnter the number of the daily item to remove: ");

	
		if (inputBuffer <= numberOfItems_)
		{
			int count = 1;
			successValue = remove(head_, inputBuffer, count);

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
			cout << "There aren't that many items on your daily item list!" << endl;
		}

	}
	

	return successValue;

}



int Day::remove(ItemNode*& head, const int& entryToRemove, int& currentEntry)
{
	
	int successValue = 0;

	if (head != nullptr)
	{
		if (currentEntry < entryToRemove)
		{
			++currentEntry;

			// We use lookahead here so that our successValue can be communicated
			if (head->getNext() != nullptr)
			{
				successValue = remove(head->getNext(),
							entryToRemove,
							currentEntry);
			}
		}
		else if (entryToRemove == currentEntry)
		{
			// Delete the node
			ItemNode* hold = head;
			head = head->getNext();

			delete hold;

			--numberOfItems_;
			successValue = 1;
		}
	}

	return successValue;
}