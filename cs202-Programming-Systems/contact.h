/*
	contact.h

	Manages Contacts, their derived classes, the Contact List and UI

	Author: Brooke Czerwinski
	Date:	3/1/2020
	Class:	CS202
	Program #3

*/

#include "device.h"


// Device Node for use in Device LLL kept under contact
class DeviceNode {
public:
	// Default constructor
	DeviceNode();
	// Copy constructor
	DeviceNode(const DeviceNode& source);
	// Contructor from Device pointer. No implicit type conversion allowed
	explicit DeviceNode(const Device* source);
	// Destructor
	~DeviceNode();

	// Receive an incoming communication pointer to the device at the node
	/*
		INPUT:	- source - Communication ptr - pointer to the incoming communication
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int receive(const Communication* source);

	// Calls the devices interface
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int interface();

	// Displays the devices details
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display() const;

	// Returns the nodes next DeviceNode pointer
	/*
		INPUT:	- NONE
		OUTPUT:	- DeviceNode ptr by ref - pointer to the next node
	*/
	DeviceNode*& getNext();

	// Returns the nodes next DeviceNode pointer - const
	/*
		INPUT:	- NONE
		OUTPUT:	- DeviceNode ptr by ref - pointer to the next node
	*/
	DeviceNode* getNext() const;

	// Set next pointer of node to argument
	/*
		INPUT:	- next - DeviceNode ptr by ref - sets next_ to this argument
		OUTPUT:	- NONE
	*/
	void setNext(DeviceNode*& next);

	// Returns a pointer by value to the data in the node
	/*
		INPUT:	- NONE
		OUTPUT:	- Device ptr - Device ptr in the node
	*/
	Device*  getData() const;

private:
	Device*		deviceData_;	// Device data in the node
	DeviceNode*	next_;			// Next DeviceNode

	// Copy the argument Device into the node's data
	/*
		INPUT:	- source - Device ptr - Device to copy into the node
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int copyData(const Device* source);
};

// enum that helps figure out what kind of device type is being used
enum DeviceType { landlineType, tabletType, smartphoneType };

class Contact {
public:
	// Default constructor
	Contact();
	// Copy constructor
	Contact(const Contact& source);
	// Constructor from contactName string. No implicit type conversion allowed
	explicit Contact(const String& contactName);

	// Destructor
	~Contact();

	// Opens the interface for the contact.
	// Displays devices
	// Allows access to devices
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int interface();

	// Add the argument Device poitner to the contact's device list
	/*
		INPUT:	- newDevice - Device ptr - new device pointer to copy and add
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int add(const Device* newDevice);

	// Outputs the contact's name to the console
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int displayName() const;

	// Displays the contact's device list with numbers
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int displayNumbered() const;

	// Returns the size of the contact's device list.
	// Assists with display in the Contact in the Contact List
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int getSize() const;

	// Returns the name by value
	// This is used very specifically when the CommUI needs to create a new
	// Text or Connection object. It must return it by value so it can be used
	// in the constructor of the Communiation
	/*
		INPUT:	- NONE
		OUTPUT:	- String by val - contactName_ data member
	*/
	String getName() const;

	// Operator Overloading
	// =, +, +=, [], ==, !=, relational/equailty, and input/output (<< and >>)

	// Assignment operator =
	Contact& operator = (const Contact& source);

	// + operator
	// Adds the Contact and device together to produce a new contact
	// that includes all of the devices from the original contact
	// plus the additional device
	friend Contact operator + (const Contact& op1, const Device* op2);
	friend Contact operator + (const Device* op1, const Contact& op2);

	// += operator
	// Adds the argument device to the calling contact's list
	Contact& operator += (const Device* source);

	// [] operator
	// Returns the nth device in the device list
	// This can segfault like it would with any other usage so you have to
	// implement your own checks
	Device* operator[](int index) const;
	Device* operator[](int index);

	// Relational/Equality operators
	// These all compare contact names (case-insensitive)
	friend bool operator < (const Contact& op1, const Contact& op2);
	friend bool operator < (const Contact& op1, const String& op2);
	friend bool operator < (const String& op1, const Contact& op2);

	friend bool operator <= (const Contact& op1, const Contact& op2);
	friend bool operator <= (const Contact& op1, const String& op2);
	friend bool operator <= (const String& op1, const Contact& op2);

	friend bool operator > (const Contact& op1, const Contact& op2);
	friend bool operator > (const Contact& op1, const String& op2);
	friend bool operator > (const String& op1, const Contact& op2);

	friend bool operator >= (const Contact& op1, const Contact& op2);
	friend bool operator >= (const Contact& op1, const String& op2);
	friend bool operator >= (const String& op1, const Contact& op2);

	friend bool operator != (const Contact& op1, const Contact& op2);
	friend bool operator != (const Contact& op1, const String& op2);
	friend bool operator != (const String& op1, const Contact& op2);

	friend bool operator == (const Contact& op1, const Contact& op2);
	friend bool operator == (const Contact& op1, const String& op2);
	friend bool operator == (const String& op1, const Contact& op2);

	// Input/Output
	// << operator (output)
	// Outputs the contact name
	friend ostream& operator << (ostream& out, const Contact& op2);
	// >> operator (input)
	// Takes in the contact name
	friend istream& operator >> (istream& in, Contact& op2);

private:
	String*		contactName_;	// Contact's name
	DeviceNode*	head_;		// Head of the contact's device list
	int		size_;		// Number of devices in the contact's device list

	// Releases all dynamic memory recursively
	/*
		INPUT:	- head - DeviceNode ptr by ref - current node
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int releaseMemory(DeviceNode*& head);

	// Copies the device log
	/*
		INPUT:	- sourceHead - DeviceNode ptr - current source node
				- destinationHead - DeviceNode ptr - current destination node
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int copyLog(const DeviceNode* sourceHead, DeviceNode*& destinationHead);

	// Displays the contact's info and devices
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display() const;

	// Displays the contact's info and devices recursively
	/*
		INPUT:	- sourceHead - DeviceNode ptr - current node that we are displaying
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display(const DeviceNode* sourceHead) const;

	// Displays teh contact's info and devices recursively
	/*
		INPUT:	- sourceHead - DeviceNode ptr - current node that we are displaying
				- currentCount - int - current number to display
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int displayNumbered(const DeviceNode* sourceHead, int& currentCount) const;

	// Interface to add a device to the contact
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int add();

	// Add a device of the argument type (landlilne, tablet, or smartphone)
	/*
		INPUT:	- aDeviceType - DeviceType by ref - device type to add
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int add(const DeviceType& aDeviceType);

	// Add a device of the argument type (landline, tablet, or smartphone) recursively
	/*
		INPUT:	- head - DeviceNode ptr by ref - current Device node
				- newDevice - Device ptr - new device to add to the list
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int add(DeviceNode*& head, const Device* newDevice);

	// Displays the interface to remove an entry
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int remove();

	// Removes an entry recursively
	/*
		INPUT:	- head - DeviceNode ptr by ref - current node
				- entryToRemove - int by ref - position number of entry to remove
				- currentEntry - int by ref - current position number of current node
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int remove(DeviceNode*& head, const int& entryToRemove, int& currentEntry);

	// Opens the interface to view a device in the list
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int viewDevice();

	// View a device in the list and navigate to it recursively
	/*
		INPUT:	- head - DeviceNode ptr by ref - current node
				- entryToView - int by ref - position number of entry to view
				- currentEntry - int by ref - current position number of current node
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int viewDevice(DeviceNode*& head, const int& entryToView, int& currentEntry);
	
	// Returns the device requested by [] overloading of the Contact class recursively
	// Used only by the CommUI class when creating a new communication object
	/*
		INPUT:	- head - DeviceNode by ref - current node
				- entryToReturn - int by ref - position number of entry to return
				- currentEntry - int by ref - current position number of curretn node
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	Device* returnDevice(DeviceNode& head, const int& entryToReturn, int& currentEntry) const;
};

// Used by the ContactBST class. Inherits from contact
class ContactVertex : public Contact {
public:
	// Default constructor
	ContactVertex();
	// Copy constructor
	ContactVertex(const ContactVertex& source);

	// Constructor from string contact name
	// Implicit type conversion not allowed
	explicit ContactVertex(const String& contactName);

	// Destructor
	~ContactVertex();

	// Returns the left pointer of the vertex
	/*
		INPUT:	- NONE
		OUTPUT:	- ContactVertex ptr by ref - left poitner of the vertex
	*/
	ContactVertex*& getLeft();

	// Returns the left poitner of the vertex - const
	/*
		INPUT:	- NONE
		OUTPUT:	- ContactVertex ptr by ref - left pointer of the vertex
	*/
	ContactVertex* getLeft() const;

	//Set the left pointer of the vertex to the argument
	/*
		INPUT:	- left - ContactVertex ptr by ref - new left pointer
		OUTPUT:	- NONE
	*/
	void setLeft(ContactVertex*& left);

	// Returns the right pointer of the vertex
	/*
		INPUT:	- NONE
		OUTPUT:	- ContactVertex ptr by ref - right pointer of the vertex
	*/
	ContactVertex*& getRight();

	// Returns the right pointer of the vertex - const
	/*
		INPUT:	- NONE
		OUTPUT:	- ContactVertex ptr by ref - right pointer of the vertex
	*/
	ContactVertex* getRight() const;

	//Set the right pointer of the vertex to the argument
	/*
		INPUT:	- right - ContactVertex ptr by ref - new right pointer
		OUTPUT:	- NONE
	*/
	void setRight(ContactVertex*& right);

private:
	ContactVertex* 	right_;	// Right child of vertex
	ContactVertex*	left_;	// Left child of vertex
};

// enum to define sender and recipient communication types
enum senderRecipient { sender, recipient };

class ContactBST {
public:
	// Default constructor
	ContactBST();
	// Copy constructor
	ContactBST(const ContactBST& source);
	// Destructor
	~ContactBST();

	// Add the argument name as a new contact in the BST
	/*
		INPUT:	- newContactName - String& - name for new contact
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int addContact(const String& newContactName);

	// Display the BST
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display() const;

protected:
	ContactVertex* root_;	// Root of the BST structure

	//  Delete the argument vetex from the BST
	/*
		INPUT:	- root - ContactVertex ptr by ref - vertex to remove from bst
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int deleteFromBST(ContactVertex*& root);

	// Find and remove the inorder successor from its current position.
	// puts the IOS into the newVertex so it can be placed where it called
	/*
		INPUT:	- root - ContactVertex ptr by ref - vertex to add to the bst
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int findDeleteIOS(ContactVertex*& root, ContactVertex*& newVertex);

	// Displays the interface to remove a contact from the BST
	/*
		INPUT:	- prompt - String by ref - prompt for input
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int removeContact(const String& prompt);

	//  Remomves a contact from teh BST recursively
	/*
		INPUT:	- root - ContactVertex ptr by ref - current vertex
				- nameToRemove - String - name of the contact to remove
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int removeContact(ContactVertex*& root, const String& nameToRemove);

	// Displays the contact list and
	// gives an interface that allows the user to enter a contact name
	// prompt will be given by argument. Returns matching contact pointer
	// Used when getting info for the creation of a Text or Connection object
	/*
		INPUT:	- prompt - String by ref - prompt to display for user
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	Contact* getContactPtr(const String& prompt);

	// Get contact Pointer recursively
	/*
		INPUT:	- root - ContactVertex - current vertex
				- contactName - String by ref - name of contact pointer to get
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	Contact* getContactPtr(ContactVertex* root, const String& contactName);

	// Get DevicePtr from argument Contact
	/*
		INPUT:	- sourceContact - Contact by ref - Contact to get a device from
				- aCommType - CommType enum - The type of communication requested
				- senderOrRecipient - String by ref - String of "sender" or "recipient"
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	Device* getDevicePtr(Contact& sourceContact,
				const CommType& aCommType,
				const String& senderOrRecipient);
	
private:
	// Deletes the BST recursively
	/*
		INPUT:	- root - ContactVertex - current vertex
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int deleteBST(ContactVertex*& root);

	// Copy a BST recursively
	/*
		INPUT:	- destinationRoot - ContactVetex ptr by ref - copy into
				- sourceRoot - ContactVertex ptr - copy from
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int copyBST(ContactVertex*& destinationRoot, const ContactVertex* sourceRoot);

	// Display the BST recursively
	/*
		INPUT:	- root - ContactVertex ptr - current vertex
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display(const ContactVertex* root) const;

	// Add argument cotnact name to the BST recursively
	/*
		INPUT:	- root - ContactVertex - current Vertex
				- newContactName - String by ref - new name to add
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int addContact(ContactVertex*& root, const String& newContactName);
};

// Manages the final level interface and creates communications
class CommUI : public ContactBST {
public:
	// Launches the main interface
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int interface();

	// Adds the argument contact name to the BST
	/*
		INPUT:	- newContactName - String by ref - New contact name to add to BST 
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int addContact(const String& newContactName);

private:
	// interface to add a new contact
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int addContact();

	//  Interface to remove a contact
	/*
		INPUT:	-  NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int removeContact();

	// Activates the interface of the requested contact
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int goToContact();

	// Deliver communication
	// Opens a menu and asks what contact you want to send the communicaiton from
	// Then will ask what device you want to send the communication from
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int send();
};
