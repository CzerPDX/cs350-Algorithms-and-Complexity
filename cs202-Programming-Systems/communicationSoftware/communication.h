/*
	communication.h

	Manages Communications and their derived classes

	Author: Brooke Czerwinski
	Date:	3/1/2020
	Class:	CS202
	Program #3

*/

#include "utilities.h"

// An enum that defines what type of communication something is
enum CommType { textType, callType };

class Communication
{
public:
	// Default constructor
	Communication();

	// Destructor (virtual because this is a virtual base class)
	virtual ~Communication();

	// Pure virtual display function
	// Never meant to be called from this class
	virtual int display() const = 0;

	// Pure virtual compare function
	// Never meant to be called from this class
	virtual int compareCommType(const CommType& aCommType) const = 0;
};


// Manages basic connection. A call is so simple it does not need its own
// class, so this is essentially the "call" class. However, connections
// could also be used as a basis for something like a GPS, so it shouldn't
// be the call class specifically.
class Connection : public Communication
{
public:
	// Default constructor
	Connection();
	// Copy constructor
	Connection(const Connection& source);
	// Constructor with arguments
	Connection(const CommType& commType,
		   const String& recipientName,
		   const String& recipientPhone,
		   const String& senderName,
		   const String& senderPhone);
	// Destructor
	~Connection();

	// Displays the recipient and sender names and phone numbers
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 for success or 0 for failure
	*/
	int display() const;

	// Compares the commTypes
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if the CommTypes are the same
			    - 0 if the CommTypes are different
	*/
	int compareCommType(const CommType& aCommType) const;

protected:
	CommType	commType_;		// Text or Connection (call)

private:
	String*		recipientName_;		// Recipient's name
	String*		recipientPhone_;	// Recipient's phone number
	String*		senderName_;		// Sender's name
	String*		senderPhone_;		// Sender's phone number

	// Releases all dynamic memory
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 for success, 0 for failure
	*/
	int releaseMemory();
};

// Manages the text messages
class Text : public Connection
{
public:
	// Default constructor
	Text();
	// Copy constructor
	Text(const Text& source);
	// Constructor from Connection
	// Implicit type conversion not allowed
	explicit Text(const Connection& source);
	Text(const CommType& commType,
		   const String& recipientName,
		   const String& recipientPhone,
		   const String& senderName,
		   const String& senderPhone);

	// Constructor from connection + text
	// would have been useful for loading from a file,
	// but loading was not implemented
	Text(const Connection& source, const String& sourceText);
	// Destructor
	~Text();

	// Requests input for textMessage_
	// This will not give a prompt for sender or reciever information
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure
	*/
	int interface();

	// Displays the text message contents
	// as well as the sender/receiver details
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display() const;

private:
	String*	textMessage_;	// Contents of the actual text message

	// Releases all dynamic memory
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int releaseMemory();
};


class CommNode
{
public:
	// Default constructor
	CommNode();
	// Copy constructor
	CommNode(const CommNode& source);
	// Constructor from communication pointer. No implicit type conversion allowed
	explicit CommNode(const Communication* source);
	// Destructor
	~CommNode();

	// Releases a pointer to the node's Communication data 
	// (either a Text or Connection (call))
	/*
		INPUT:	- NONE
		OUTPUT:	- Communication pointer - returns a poitner to the communication data in the node.
	*/
	Communication*& getData();

	// Sets the node's communication data pointer to the argument pointer
	/*
		INPUT:	- data - Communication pointer - pointer to set as data
		OUTPUT:	- NONE
	*/
	void setData(Communication*& data);

	// Returns the node's next pointer
	/*
		INPUT:	- NONE
		OUTPUT:	- CommNode pointer - pointer to the next CommNode
	*/
	CommNode*& getNext();

	// Return the node's next pointer (const)
	/*
		INPUT:	- NONE
		OUTPUT:	- CommNode pointer - pointer to the next CommNode
	*/
	CommNode* getNext() const;

	// Sets the node's next pointer to the argument
	/*
		INPUT:	- next - CommNode pointer - next CommNode to point to
		OUTPUT:	- NONE
	*/
	void setNext(CommNode*& next);

	// Displays the contents of the communications data
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display() const;

private:
	Communication* 	commData_;	// The pointer to the text or connection (call)
	CommNode*	next_;		// Pointer to the next CommNode in the LLL

	// Copies all the data from the argument Communication
	// into the calling node
	/*
		INPUT:	- source - Communication pointer - communication you want copied into the calling node
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int copyData(const Communication* source);

	// Releases all dynamic memory
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int releaseMemory();
};

// A LLL of communication pointers managed with dynamic binding
// Communication pointers could hold texts or calls 
// (or other theoretical kinds of communication like GPS)
class CommLog
{
public:
	// Default constructor
	CommLog();
	// Copy constructor
	CommLog(const CommLog& source);
	// Destructor
	~CommLog();

	// Displays log and allows you to remove text from log or q to go back.
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int interface();

	// Take in a new communication pointer to copy and add to the list
	/*
		INPUT:	- source - Communication pointer - new communication to add to the CommLog
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int add(const Communication* source);

private:
	CommNode* 	head_;	// Head of the LLL of CommNodes
	int			size_;	// Number of CommNodes in the CommLog

	// Copies the argument log into the calling log recursively
	/*
		INPUT:	- sourceHead - CommNode ptr - Current node in the source LLL
				- destinationHead - CommNode ptr - Current node in the dest LLL
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int copyLog(const CommNode* sourceHead, CommNode*& destinationHead);

	// Releases all dynamic memory recursively
	/*
		INPUT:	- head - CommNode ptr by reference - pointer to the head of the CommLog to delete
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int releaseMemory(CommNode*& head);

	// Add a new Communication to the CommLog recursively
	/*
		INPUT:	- head - CommNode ptr by reference - head of the CommLog to add new Communication to
				- source - Communication ptr - Communication to copy and add to the CommLog
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int add(CommNode*& head, const Communication* source);

	// Displays the Communications in the CommLog
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display() const;

	// Displays the Communications in the CommLog recursively
	/*
		INPUT:	- sourceHead - CommNode ptr - Head of the CommLog to display
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display(const CommNode* sourceHead) const;

	// Displays the Communications in the CommLog numbered
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int displayNumbered() const;

	// Displays the Communications in the CommLog numbered recursively
	/*
		INPUT:	- sourceHead - CommNode ptr - head of the CommLog to display
				- currentCount - int by ref - Current number to display
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int displayNumbered(const CommNode* sourceHead, int& currentCount) const;

	// Interface to remove a log entry by its position number
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int remove();

	// Removes a log entry by its position number recursively
	/*
		INPUT:	- head - CommNode ptr - head of the CommLog to remove from
				- entryToRemove - int by ref - position number of the entry to remove
				- current entry - int by ref - current position number of current node
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int remove(CommNode*& head, const int& entryToRemove, int& currentEntry);	
};
