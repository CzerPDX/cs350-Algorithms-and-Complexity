/*
	communication.cpp

	Manages Communications and their derived classes

	Author: Brooke Czerwinski
	Date:	3/1/2020
	Class:	CS202
	Program #3

*/

#include "communication.h"

using std::cout;
using std::endl;
using std::cin;

// Communication class
// Default constructor
Communication::Communication() {}
Communication::~Communication() {}


// Connection class
// Default constructor
Connection::Connection() : 
		commType_(callType),
		recipientName_(nullptr), 
		recipientPhone_(nullptr), 
		senderName_(nullptr), 
		senderPhone_(nullptr) {}

// Copy constructor
Connection::Connection(const Connection& source) : 
				commType_(source.commType_),
				recipientName_(nullptr),
				recipientPhone_(nullptr),
				senderName_(nullptr),
				senderPhone_(nullptr) {
	if (source.recipientName_ != nullptr) {
		recipientName_ = new String(*source.recipientName_);
	}

	if (source.recipientPhone_ != nullptr) {
		recipientPhone_ = new String(*source.recipientPhone_);
	}

	if (source.senderName_ != nullptr) {
		senderName_ = new String(*source.senderName_);
	}

	if (source.senderPhone_ != nullptr) {
		senderPhone_ = new String(*source.senderPhone_);
	}
}

// Constructor with arguments
Connection::Connection(const CommType& commType,
			const String& recipientName,
			const String& recipientPhone,
			const String& senderName,
			const String& senderPhone) :
			commType_(commType),
			recipientName_(nullptr),
			recipientPhone_(nullptr),
			senderName_(nullptr),
			senderPhone_(nullptr) {
	recipientName_ = new String(recipientName);
	recipientPhone_ = new String(recipientPhone);
	senderName_ = new String(senderName);
	senderPhone_ = new String(senderPhone);
}

// Destructor
Connection::~Connection() {
	releaseMemory();
}

// Release dynamic memory
int Connection::releaseMemory() {
	int successValue = 0;

	if (recipientName_ != nullptr) {
		delete recipientName_;
		recipientName_ = nullptr;
	}
	if (recipientPhone_ != nullptr) {
		delete recipientPhone_;
		recipientPhone_ = nullptr;
	}

	if (senderName_ != nullptr) {
		delete senderName_;
		senderName_ = nullptr;
	}
	if (senderPhone_ != nullptr) {
		delete senderPhone_;
		senderPhone_ = nullptr;
	}

	commType_ = callType;

	if ((recipientName_ == nullptr) && 
		(recipientPhone_ == nullptr) &&
		(senderName_ == nullptr) &&
		(senderPhone_ == nullptr)) {
		successValue = 1;
	}

	return successValue;
}

// Display data members
int Connection::display() const {
	int successValue = 0;

	if ((recipientName_ != nullptr) && (senderName_ != nullptr)) {
		if (commType_ == callType) {
			cout << "Call ";
		}
		else {
			cout << "Text ";
		}
		cout << "From: " << *senderName_ << " " << *senderPhone_;
		cout << ", To: " << *recipientName_ << " " << *recipientPhone_ << endl;

		successValue = 1;
	}

	return successValue;
}

int Connection::compareCommType(const CommType& aCommType) const {
	return (commType_ == aCommType);
}


// Text class
// Default constructor
Text::Text() : textMessage_(nullptr) {}

// Copy constructor
Text::Text(const Text& source) : Connection(source), textMessage_(nullptr) {
	if (source.textMessage_ != nullptr) {
		textMessage_ = new String(source.textMessage_);
	}

	commType_ = textType;
}

// Secondary constructors
Text::Text(const Connection& source) : Connection(source), textMessage_(nullptr) {}

Text::Text(const Connection& source, const String& sourceText) :
					Connection(source),
					textMessage_(nullptr) {
	textMessage_ = new String(sourceText);

	commType_ = textType;
}

Text::Text(const CommType& commType,
		const String& recipientName,
		const String& recipientPhone,
		const String& senderName,
		const String& senderPhone) :
		Connection(commType,
				recipientName,
				recipientPhone,
				senderName,
				senderPhone),
				textMessage_(nullptr){}

Text::~Text() {
	releaseMemory();
}

int Text::releaseMemory() {
	int successValue = 0;

	if (textMessage_ != nullptr) {
		delete textMessage_;
		textMessage_ = nullptr;
	}

	if (textMessage_ == nullptr) {
		successValue = 1;
	}

	return successValue;
}

int Text::interface() {
	int successValue = 0;
	String	prompt;
	char	temp[256];

	releaseMemory();
	commType_ = textType;
	prompt = "Please enter your text message: ";
	takeInput(temp, prompt);
	textMessage_ = new String(temp);
	
	return successValue;
}

int Text::display() const {
	int successValue = 0;

	if (textMessage_ != nullptr) {
		Connection::display();
		cout << *textMessage_ << endl;
	}

	return successValue;
}


// CommNode class
// Default constructor
CommNode::CommNode() : commData_(nullptr), next_(nullptr) {}

// Copy constructor
CommNode::CommNode(const CommNode& source) : commData_(nullptr), next_(nullptr) {
	// Copy the data info
	copyData(source.commData_);
}

CommNode::CommNode(const Communication* source) : commData_(nullptr), next_(nullptr) {
	// Copy the data
	copyData(source);
}

CommNode::~CommNode() {
	releaseMemory();
}

int CommNode::releaseMemory() {
	int successValue = 0;

	if (commData_ != nullptr) {
		delete commData_;
		commData_ = nullptr;
	}

	if (next_ != nullptr) {
		next_ = nullptr;
	}

	if ((commData_ == nullptr) && (next_ == nullptr)) {
		successValue = 1;
	}

	return successValue;
}

// Copy argument commData_. Assists copy constructor
int CommNode::copyData(const Communication* source) {
	int successValue = 0;

	if (source != nullptr) {
		// Try to save the item as a Text, first
		const Text* textPtr = dynamic_cast<const Text*>(source);

		if (textPtr != nullptr) {
			commData_ = new Text(*textPtr);
			successValue = 1;
		}
		// Otherwise it is a Connection type of communication (a call)
		else {
			const Connection* connectionPtr = dynamic_cast<const Connection*>(source);
			if (connectionPtr != nullptr) {
				commData_ = new Connection(*connectionPtr);
				successValue =1;
			}
		}
	}

	return successValue;
}

Communication*& CommNode::getData() {
	return commData_;
}

void CommNode::setData(Communication*& newCommData) {
	copyData(newCommData);
}

CommNode*& CommNode::getNext() {
	return next_;
}

CommNode* CommNode::getNext() const {
	return next_;
}

void CommNode::setNext(CommNode*& newNext) {
	next_ = newNext;
}

int CommNode::display() const {
	int successValue;

	successValue = commData_->display();	

	return successValue;
}


// CommLog class
// Default constructor
CommLog::CommLog() : head_(nullptr), size_(0) {}

// Copy constructor
CommLog::CommLog(const CommLog& source) : head_(nullptr), size_(source.size_) {
	copyLog(source.head_, head_);
}

// Destructor
CommLog::~CommLog() {
	releaseMemory(head_);
}

int CommLog::releaseMemory(CommNode*& head) {
	int successValue = 0;

	if (head != nullptr) {
		successValue = releaseMemory(head->getNext());

		delete head;
		head = nullptr;
		successValue = 1;
		--size_;
	}

	return successValue;
}

int CommLog::copyLog(const CommNode* sourceHead, CommNode*& destinationHead) {
	int successValue = 0;

	if (sourceHead != nullptr) {
		destinationHead = new CommNode(*sourceHead);

		if (sourceHead->getNext() == nullptr) {
			successValue = 1;
		}
		else {
			successValue = copyLog(sourceHead->getNext(), destinationHead->getNext());
		}
	}

	return successValue;
}

int CommLog::interface() {
	int 	successValue = 0;
	char 	inputBuffer = 0;

	String	prompt;
	prompt = "1. Remove something from the log\nq. Go back to the previous menu.\n\nEnter your choice: "; 

	do {
		successValue = display();
		takeInput(inputBuffer, prompt);

		switch(inputBuffer) {
			case '1':
				remove();
				break;
			case 'q':
				cout << "Returning..." << endl << endl;
				break;
			default:
				cout << "Invalid entry. Please try again..." << endl;
				cout << endl;
		}
	} while(quitCheck(inputBuffer) != true);

	return successValue;
}

int CommLog::add(const Communication* source) {
	return add(head_, source);
}

int CommLog::add(CommNode*& head, const Communication* source) {
	int successValue = 0;

	if (source != nullptr) {
		// We want to add to the list if the head is null
		if (head == nullptr) {
			head = new CommNode(source);
			++size_;
			successValue = 1;
		}
		// Otherwise keep traversing till the end
		else {
			successValue = add(head->getNext(), source);
		}
	}

	return successValue;
}

int CommLog::remove() {
	int 	successValue = 1;
	int		inputBuffer = 0;
	String	prompt;
	prompt = "\nEnter the number of the list item to remove: "; 

	cout << endl;
	cout << "Remove A Communication From the History" << endl;
	cout << "=========================================" << endl;
	successValue = displayNumbered();

	if (successValue != 0) {
		takeInput(inputBuffer, prompt);
	
		if ((inputBuffer <= size_) && (inputBuffer > 0)) {
			int count = 1;
			successValue = remove(head_, inputBuffer, count);

			if (successValue != 0) {
				cout << endl;
				cout << "Success!" << endl << endl;
			}
			else {
				cout << endl;
				cout << "Failed to remove the entry from the log";
				cout << endl << endl;
			}
		}
		else {
			cout << endl;
			cout << "Invalid entry. Returning to previous menu..." << endl << endl;
		}
	}
	else {
		cout << "Log is empty! Nothing to remove. Returning..." << endl << endl;
	}

	return successValue;
}

int CommLog::remove(CommNode*& head, const int& entryToRemove, int& currentEntry) {
	int successValue = 0;

	if (head != nullptr) {
		if (currentEntry < entryToRemove) {
			++currentEntry;

			// We use lookahead here so that our successValue can be communicated
			if (head->getNext() != nullptr) {
				successValue = remove(head->getNext(), entryToRemove, currentEntry);
			}
		}
		else if (entryToRemove == currentEntry) {
			CommNode* hold = head;
			head = head->getNext();

			delete hold;
			--size_;

			successValue = 1;
		}
	}

	return successValue;
}

int CommLog::display() const {
	int successValue = 0;

	if (head_ != nullptr) {
		successValue = display(head_);
	}
	else {
		cout << "Communication Log is empty!" << endl;
		cout << endl;
	}
	return successValue;
}

int CommLog::display(const CommNode* sourceHead) const {
	int successValue = 0;

	if (sourceHead != nullptr) {
		sourceHead->display();
		cout << endl;

		if (sourceHead->getNext() == nullptr) {
			successValue = 1;
		}
		else {
			successValue = display(sourceHead->getNext());
		}
	}

	return successValue;
}

int CommLog::displayNumbered() const {
	int startNumberingFrom = 1;
	return displayNumbered(head_, startNumberingFrom);
}

int CommLog::displayNumbered(const CommNode* sourceHead, int& currentCount) const {
	int successValue = 0;

	if (sourceHead != nullptr) {
		cout << currentCount << ". ";
		++currentCount;
		sourceHead->display();
		cout << endl;

		if (sourceHead->getNext() == nullptr) {
			successValue = 1;
		}
		else {
			successValue = displayNumbered(sourceHead->getNext(), currentCount);
		}
	}

	return successValue;
}
