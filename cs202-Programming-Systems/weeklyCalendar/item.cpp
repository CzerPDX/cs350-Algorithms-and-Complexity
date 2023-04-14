/*

        item.cpp

        Manage the Item class and its derived classes

        Author: Brooke Czerwinski
        Date: 	2/3/2021
        Class: 	CS 202
        Program #2

*/


#include "item.h"


using std::cout;
using std::endl;


//////////////////////////////////////////////////
// Item class

// Default Constructor
Item::Item() {}

// Destructor
Item::~Item() {}


//////////////////////////////////////////////////
// GroceryItem clas

// Default Constructor
GroceryItem::GroceryItem() : name_(nullptr), qty_(0) {}

// Copy Constructor
GroceryItem::GroceryItem(const GroceryItem& source) : name_(nullptr), qty_(source.qty_)
{
	if (source.name_ != nullptr)
	{
		name_ = new char[strlen(source.name_) + 1];
		strcpy(name_, source.name_);
	}
}

// Secondary Constructor
GroceryItem::GroceryItem(const char* name,const int& qty) : name_(nullptr), qty_(qty)
{
	if (name != nullptr)
	{
		name_ = new char[strlen(name) + 1];
		strcpy(name_, name);
	}
}


GroceryItem::~GroceryItem()
{
	clearValues();
}


int GroceryItem::clearValues()
{
	int successValue;

	if (name_ != nullptr)
	{
		delete[] name_;
		name_ = nullptr;
	}

	qty_ = 0;

	if (name_ == nullptr)
	{
		successValue = 1;
	}

	return successValue;
}


int GroceryItem::display() const
{
	int successValue = 0;

	if (name_ != nullptr)
	{
		cout << qty_ << " " << name_;

		successValue = 1;
	}

	return successValue;
}

int GroceryItem::interface()
{
	int 	successValue = 0;
	char	inputBuffer[MAX_CHAR];
	int 	stringLength = 0;
	
	// Take the grocery item's name
	takeInput(inputBuffer, "Please enter the name of the grocery item: ");
	stringLength = strlen(inputBuffer);

	successValue = clearValues();
	if (successValue != 0)
	{
		name_ = new char[stringLength + 1];
		strcpy(name_, inputBuffer);
	}

	// Take the quantity
	// This one doesn't need an input buffer because it is not dynamic memory
	takeInput(qty_, "Quantity: ");


	return successValue;	
}


int GroceryItem::copy(GroceryItem& destination)
{
	int successValue = 0;

	if (destination.name_ != nullptr)
	{
		delete[] destination.name_;
		destination.name_ = nullptr;
	}
	destination.name_ = new char[strlen(name_) + 1];
	strcpy(destination.name_, name_);

	destination.qty_ = qty_;

	successValue = 1;

	return successValue;
}




//////////////////////////////////////////////////
// GroceryNode class

// Default Constructor
GroceryNode::GroceryNode() : next_(nullptr) {}

// Copy constructor
GroceryNode::GroceryNode(const GroceryNode& source) : GroceryItem(source), next_(nullptr)
{
	if (source.next_ != nullptr)
	{
		next_ = new GroceryNode(*source.next_);
	}
}


// Secondary constructor
GroceryNode::GroceryNode(const GroceryItem& source) : GroceryItem(source), next_(nullptr) {}

// Destructor
GroceryNode::~GroceryNode()
{
	clearValues();
}



int GroceryNode::clearValues()
{
	int successValue = 0;

        if (next_ != nullptr)
        {
                next_ = nullptr;
        }

        if (next_ == nullptr)
        {
                successValue = 1;
        }

        return successValue;
}



void GroceryNode::setNext(GroceryNode*& next)
{
        next_ = next;
}


GroceryNode*& GroceryNode::getNext()
{
        return next_;
}


int GroceryNode::copyWrapper(GroceryItem& destination)
{
	return copy(destination);
}




//////////////////////////////////////////////////
// GroceryList class

// Default Constructor
GroceryList::GroceryList() : head_(nullptr), size_(0) {}

// Copy constructor
GroceryList::GroceryList(const GroceryList& source) : head_(nullptr), size_(source.size_)
{
	if (source.head_ != nullptr)
	{
		head_ = new GroceryNode(*source.head_);
	}
}



GroceryList::GroceryList(const GroceryNode* source) : head_(nullptr), size_(0)
{
	if (source != nullptr)
	{
		head_ = new GroceryNode(*source);
		++size_;
	}
}


GroceryList::~GroceryList()
{
	clearValues(head_);
}


int GroceryList::clearValues()
{
	return clearValues();
}


int GroceryList::clearValues(GroceryNode*& head)
{
	int successValue = 0;

	if (head != nullptr)
	{
		successValue = clearValues(head->getNext());

		delete head;
		head = nullptr;
		successValue = 1;
	}

	if (head == nullptr)
	{
		successValue = 1;
	}

	return successValue;
}



int GroceryList::interface()
{
	int 	successValue = 0;
	char	inputBuffer;


	
	// Take the grocery item's name
	do
	{
		cout << endl << endl;
		cout << "Grocery List" << endl;
		cout << "============" << endl;
		display();
		

		takeInput(inputBuffer, "\nWhat would you like to do:\n\n1. Add to the list\n2. Remove from the list\nq. Go back\n\nEnter your choice: ");

		switch(inputBuffer)
		{
			case '1':
				add();
				break;
			case '2':
				remove();
				break;
			case 'q':
				cout << "Returning..." << endl;
		}

		successValue = 1;

	} while(quitCheck(inputBuffer) != true);
	

	// Take the quantity
	// This one doesn't need an input buffer because it is not dynamic memory
	

	return successValue;
}


int GroceryList::display() const
{
	int successValue = 0;

	if (head_ != nullptr)
	{
		cout << "Grocery List: " << endl;
		successValue = display(head_);
	}
	else
	{
		cout << "Grocery list is empty!!" << endl;
	}
	return successValue;
}



int GroceryList::display(GroceryNode* head) const
{
        int successValue = 0;

        if (head != nullptr)
        {
                head->display();
		cout << endl;
		successValue = 1;

		if (head->getNext() != nullptr)
		{
			successValue = display(head->getNext());
		}
	}


        return successValue;
}


int GroceryList::displayNumbered() const
{
	int successValue = 0;
	int startingNumber = 1;
	cout << endl;

	if (head_ != nullptr)
	{
		successValue = display(head_, startingNumber);
	}
	else
	{
		cout << "Grocery list is empty!!" << endl;
	}
	return successValue;
}


int GroceryList::display(GroceryNode* head, int& currentEntry) const
{
        int successValue = 0;

        if (head != nullptr)
        {
		cout << currentEntry << ". ";
                head->display();
		cout << endl;

		successValue = 1;

		if (head->getNext() != nullptr)
		{
			++currentEntry;
			successValue = display(head->getNext(), currentEntry);
		}
        }

        return successValue;
}


int GroceryList::getSize() const
{
	return size_;
}


int GroceryList::add()
{
	int 		successValue = 0;
	GroceryItem	newItem;

	successValue = newItem.interface();

	if (successValue != 0)
	{
		successValue = add(head_, newItem);
	}

	cout << endl;

	if (successValue != 0)
	{
		cout << "Successfully added ";
		newItem.display();
		cout << endl;
	}
	else
	{
		cout << "Failed to add ";
		newItem.display();
		cout << endl;
	}

	return successValue;
}


int GroceryList::add(const GroceryItem& newItem)
{
	return add(head_, newItem);
}

int GroceryList::add(GroceryNode*& head, const GroceryItem& newItem)
{
	int successValue = 0;
	
	if (head != nullptr)
	{
		successValue = add(head->getNext(), newItem);
	}
	else
	{
		head = new GroceryNode(newItem);
		++size_;

		successValue = 1;
	}

	return successValue;
}

int GroceryList::remove()
{
	
	int 		successValue = 0;
	GroceryItem	returnedItem;
	int		inputBuffer = 0;

	successValue = displayNumbered();

	if (successValue != 0)
	{
		takeInput(inputBuffer, "\nEnter the number of the list item to remove: ");

	
		if (inputBuffer <= size_)
		{
			int count = 1;
			successValue = remove(head_, inputBuffer, count, returnedItem);

			if (successValue != 0)
			{
				cout << endl;
				cout << "Successfully removed ";
				returnedItem.display();
				cout << endl;
			}
			else
			{
				cout << endl;
				cout << "Failed to remove the item from the list";
				cout << endl;
			}
		}
		else
		{
			cout << endl;
			cout << "There aren't that many items on your grocery list!" << endl;
		}

	}
	

	return successValue;
}


int GroceryList::remove(GroceryNode*& head,
				const int& entryToRemove,
				int& currentEntry,
				GroceryItem& returnedItem)
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
							currentEntry,
							returnedItem);
			}
		}
		else if (entryToRemove == currentEntry)
		{
			head->copyWrapper(returnedItem);
			GroceryNode* hold = head;
			head = head->getNext();


			delete hold;
			--size_;

			successValue = 1;
		}
	}

	return successValue;
}






//////////////////////////////////////////////////
// School class

// Default Constructor
School::School() : className_(nullptr), homework_(nullptr), notes_(nullptr) {}

// Copy constructor
School::School(const School& source) : className_(nullptr),
					homework_(nullptr), 
					notes_(nullptr)
{
	if (source.className_ != nullptr)
	{
		className_ = new char[strlen(source.className_) + 1];
		strcpy(className_, source.className_);
	}

	if (source.homework_ != nullptr)
	{
		homework_ = new char[strlen(source.homework_) + 1];
		strcpy(homework_, source.homework_);
	}

	if (source.notes_ != nullptr)
	{
		notes_ = new char[strlen(source.notes_) + 1];
		strcpy(notes_, source.notes_);
	}
}


// Secondary constructor
School::School(const char className[],
		const char homework[],
		const char notes[]) :
		className_(nullptr),
		homework_(nullptr),
		notes_(nullptr)
{
	className_ = new char[strlen(className) + 1];
	strcpy(className_, className);

	homework_ = new char[strlen(homework) + 1];
	strcpy(homework_, homework);

	notes_ = new char[strlen(notes) + 1];
	strcpy(notes_, notes);
}


School::~School()
{
	clearValues();
}



int School::clearValues()
{
	int successValue = 0;

	if (className_ != nullptr)
	{
		delete[] className_;
		className_ = nullptr;
	}

	if (homework_ != nullptr)
	{
		delete[] homework_;
		homework_ = nullptr;
	}

	if (notes_ != nullptr)
	{
		delete[] notes_;
		notes_ = nullptr;
	}

	if ((className_ == nullptr) && (homework_ == nullptr) && (notes_ == nullptr))
	{
		successValue = 1;
	}

	return successValue;
}




int School::interface()
{
	int 	successValue = 0;
	char	inputBuffer[MAX_CHAR];
	int 	stringLength = 0;
	
	// Take the name of the class
	takeInput(inputBuffer, "What class is this for: ");
	stringLength = strlen(inputBuffer);

	successValue = clearValues();
	if (successValue != 0)
	{
		className_ = new char[stringLength + 1];
		strcpy(className_, inputBuffer);
	}

	takeInput(inputBuffer, "What do you need to do for the class: ");
	stringLength = strlen(inputBuffer);

	if (stringLength != 0)
	{
		homework_ = new char[stringLength + 1];
		strcpy(homework_, inputBuffer);
	}

	takeInput(inputBuffer, "Details: ");
	stringLength = strlen(inputBuffer);

	{
		notes_ = new char[stringLength + 1];
		strcpy(notes_, inputBuffer);
	}


	return successValue;	
}




int School::display() const
{
	int successValue = 0;

	if ((className_ != nullptr) && (homework_ != nullptr) && (notes_ != nullptr))
	{
		cout << className_ << endl;
		cout << homework_ << endl;
		cout << "Additional Info: " << endl;
		cout << notes_ << endl;

		successValue = 1;
	}

	return successValue;
}



int School::displayHomework() const
{
	int successValue = 0;

	if (homework_ != nullptr)
	{
		cout << homework_;
	}

	return successValue;
}





//////////////////////////////////////////////////
// Appointments class

// Default Constructor
Appointment::Appointment() : time_(nullptr), appointment_(nullptr), notes_(nullptr) {}

// Copy constructor
Appointment::Appointment(const Appointment& source) : time_(nullptr),
							appointment_(nullptr),
							notes_(nullptr)
{
	if (source.time_ != nullptr)
	{
		time_ = new char[strlen(source.time_) + 1];
		strcpy(time_, source.time_);
	}

	if (source.appointment_ != nullptr)
	{
		appointment_ = new char[strlen(source.appointment_) + 1];
		strcpy(appointment_, source.appointment_);
	}

	if (source.notes_ != nullptr)
	{
		notes_ = new char[strlen(source.notes_) + 1];
		strcpy(notes_, source.notes_);
	}
}


// Secondary constructor
Appointment::Appointment(const char time[],
			const char appointment[],
			const char notes[]) : 
			time_(nullptr),
			appointment_(nullptr),
			notes_(nullptr)
{
	time_ = new char[strlen(time) + 1];
	strcpy(time_, time);

	appointment_ = new char[strlen(appointment) + 1];
	strcpy(appointment_, appointment);

	notes_ = new char[strlen(notes) + 1];
	strcpy(notes_, notes);
}



Appointment::~Appointment()
{
	clearValues();
}


int Appointment::clearValues()
{
	int successValue = 0;

	if (time_ != nullptr)
	{
		delete[] time_;
		time_ = nullptr;
	}

	if (appointment_ != nullptr)
	{
		delete[] appointment_;
		appointment_ = nullptr;
	}

	if (notes_ != nullptr)
	{
		delete[] notes_;
		notes_ = nullptr;
	}

	if ((time_ == nullptr) && (appointment_ == nullptr) && (notes_ == nullptr))
	{
		successValue = 1;
	}

	return successValue;
}



int Appointment::interface()
{
	int 	successValue = 0;
	char	inputBuffer[MAX_CHAR];
	int 	stringLength = 0;
	
	// Take the name of the class
	takeInput(inputBuffer, "What is the appointment for: ");
	stringLength = strlen(inputBuffer);

	successValue = clearValues();
	if (successValue != 0)
	{
		appointment_ = new char[stringLength + 1];
		strcpy(appointment_, inputBuffer);
	}

	takeInput(inputBuffer, "What time is the appointment: ");
	stringLength = strlen(inputBuffer);

	if (stringLength != 0)
	{
		time_ = new char[stringLength + 1];
		strcpy(time_, inputBuffer);
	}

	takeInput(inputBuffer, "Details: ");
	stringLength = strlen(inputBuffer);

	if (stringLength != 0)
	{
		notes_ = new char[stringLength + 1];
		strcpy(notes_, inputBuffer);
	}


	return successValue;	
}



int Appointment::display() const
{
	int successValue = 0;

	if ((time_ != nullptr) && (appointment_ != nullptr) && (notes_ != nullptr))
	{
		cout << appointment_ << endl;
		cout << "at " << time_  << endl;
		cout << "Additional Info: " << endl;
		cout << notes_ << endl;

		successValue = 1;
	}

	return successValue;
}




int Appointment::displayAppointment() const
{
	int successValue = 0;
	
	if (appointment_ != nullptr)
	{
		cout << appointment_;
	}

	return successValue;
}



int Appointment::displayTime() const
{
	int successValue = 0;
	
	if (appointment_ != nullptr)
	{
		cout << time_;
	}

	return successValue;
}
