/*
	contact.cpp

	Manages Contacts, their derived classes, the Contact List and UI

	Author: Brooke Czerwinski
	Date:	3/1/2020
	Class:	CS202
	Program #3

*/

#include "contact.h"

using std::cout;
using std::endl;
using std::cin;


// DeviceNode class
// Default constructor
DeviceNode::DeviceNode() : deviceData_(nullptr), next_(nullptr) {}

// Copy constructor
DeviceNode::DeviceNode(const DeviceNode& source) : deviceData_(nullptr), next_(nullptr) {
	// Copy the data
	copyData(source.deviceData_);
}

// Secondary constructor
DeviceNode::DeviceNode(const Device* source) : deviceData_(nullptr), next_(nullptr) {
	// Copy the data
	copyData(source);
}

// Destructor
DeviceNode::~DeviceNode() {
	if (deviceData_ != nullptr) {
		delete deviceData_;
		deviceData_ = nullptr;
	}

	if (next_ != nullptr) {
		next_ = nullptr;
	}
}

// Copy argument deviceData_. Assists copy constructor
int DeviceNode::copyData(const Device* source) {
	int successValue = 0;

	if (source != nullptr) {
		// Try to save the Device ptr as a Landline first
		const Landline* landlinePtr = dynamic_cast<const Landline*>(source);

		if (landlinePtr != nullptr) {
			deviceData_ = new Landline(*landlinePtr);
			successValue = 1;
		}
		// Otherwise if it is not a landline
		// try to save the Device as a Tablet
		else {
			const Tablet* tabletPtr = dynamic_cast<const Tablet*>(source);

			if (tabletPtr != nullptr) {
				deviceData_ = new Tablet(*tabletPtr);
				successValue = 1;
			}
			// Otherwise if the it is not a landline or tablet
			// try to save the Device as a smartphone
			else {
				const Smartphone* smartphonePtr = dynamic_cast<const Smartphone*>(source);

				if (smartphonePtr != nullptr) {
					deviceData_ = new Smartphone(*smartphonePtr);
					successValue = 1;
				}
			}
		}
	}

	return successValue;
}

int DeviceNode::receive(const Communication* source) {
	int successValue = 0;

	if (source != nullptr) {
		successValue = deviceData_->receive(source);
	}

	return successValue;
}

int DeviceNode::interface() {
	return deviceData_->interface();
}

int DeviceNode::display() const {
	int successValue = 0;

	if (deviceData_ != nullptr) {
		deviceData_->display();
		successValue = 1;
	}

	return successValue;
}

DeviceNode*& DeviceNode::getNext() {
	return next_;
}

DeviceNode* DeviceNode::getNext() const {
	return next_;
}

void DeviceNode::setNext(DeviceNode*& newNext) {
	next_ = newNext;
}

Device* DeviceNode::getData() const {
	return deviceData_;
}


// Contact class
// Default constructor
Contact::Contact() : contactName_(nullptr), head_(nullptr), size_(0) {}

// Copy constructor
Contact::Contact(const Contact& source) : contactName_(nullptr), head_(nullptr), size_(source.size_) {
	if (source.contactName_ != nullptr) {
		contactName_ = new String(*source.contactName_);
	}

	if (source.head_ != nullptr) {
		copyLog(source.head_, head_);
	}
}

Contact::Contact(const String& contactName) : contactName_(nullptr), head_(nullptr), size_(0) {
	contactName_ = new String(contactName);
}

Contact::~Contact() {
	if (contactName_ != nullptr) {
		delete contactName_;
		contactName_ = nullptr;
	}

	if (head_ != nullptr) {
		releaseMemory(head_);
	}
}

int Contact::releaseMemory(DeviceNode*& head) {
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

int Contact::copyLog(const DeviceNode* sourceHead, DeviceNode*& destinationHead) {
	int successValue = 0;

	if (sourceHead != nullptr) {
		destinationHead = new DeviceNode(*sourceHead);

		if (sourceHead->getNext() == nullptr) {
			successValue = 1;
		}
		else {
			successValue = copyLog(sourceHead->getNext(), destinationHead->getNext());
		}
	}

	return successValue;
}


// Operator overloading for the Contact Class

// Assignment operator overloading
// Returning by reference because it's an lvalue
Contact& Contact::operator = (const Contact& source) {
	// Check for self assignment
	if (this != &source) {
		// If we already have value in our contactName_ delete it
		if (contactName_ != nullptr) {
			delete contactName_;
			contactName_ = nullptr;
		}
		
		// If there is anything in the source contact name copy it
		if (source.contactName_ != nullptr) {
			contactName_ = new String(source.contactName_);
		}

		// If our device list isn't already empty delete it.
		if (head_ != nullptr) {
			releaseMemory(head_);
		}

		// Copy the source device log over
		if (source.head_ != nullptr) {
			copyLog(source.head_, head_);
		}
	}

	return *this;
}

// + operator overloading: contact + device
Contact operator + (const Contact& op1, const Device* op2) {
	// Create a copy of the contact and add the device to it
	Contact	temp(op1);
	// add the device to the new temp contact
	temp.add(op2);
	
	return Contact(temp);
}

// + operator overloading: device + contact
Contact operator + (const Device* op2, const Contact& op1) {
	// Create a copy of the contact and add the device to it
	Contact	temp(op1);
	// add the device to the new temp contact
	temp.add(op2);
	
	return Contact(temp);
}

// += operator overloading
Contact& Contact::operator += (const Device* source) {
	add(source);

	return *this;
}

// [] operator overloading
// Const version
Device* Contact::operator[](int index) const {
	Device*	returnPtr = nullptr;

	if ((head_ != nullptr) && (index <= size_)) {
		int firstNumber = 1;
		returnPtr = returnDevice(*head_, index, firstNumber);
	}

	return returnPtr;
}

// [] operator overloading
// non-const version
Device* Contact::operator[](int index) {
	Device* returnPtr = nullptr;

	if ((head_ != nullptr) && (index <= size_)) {
		int firstNumber = 1;
		returnPtr = returnDevice(*head_, index, firstNumber);
	}

	return returnPtr;
}

// < operator overloading
bool operator < (const Contact& op1, const Contact& op2) {
	return (*op1.contactName_ < *op2.contactName_);
}

bool operator < (const String& op1, const Contact& op2) {
	return (op1 < *op2.contactName_);
}

bool operator < (const Contact& op1, const String& op2) {
	return (*op1.contactName_ < op2);
}


// <= operator overloading
bool operator <= (const Contact& op1, const Contact& op2) {
	return (*op1.contactName_ <= *op2.contactName_);
}

bool operator <= (const String& op1, const Contact& op2) {
	return (op1 <= *op2.contactName_);
}

bool operator <= (const Contact& op1, const String& op2) {
	return (*op1.contactName_ <= op2);
}


// > operator overloading
bool operator > (const Contact& op1, const Contact& op2) {
	return (*op1.contactName_ > *op2.contactName_);
}

bool operator > (const String& op1, const Contact& op2) {
	return (op1 > *op2.contactName_);
}

bool operator > (const Contact& op1, const String& op2) {
	return (*op1.contactName_ > op2);
}


// >= operator overloading
bool operator >= (const Contact& op1, const Contact& op2) {
	return (*op1.contactName_ >= *op2.contactName_);
}

bool operator >= (const String& op1, const Contact& op2) {
	return (op1 >= *op2.contactName_);
}

bool operator >= (const Contact& op1, const String& op2) {
	return (*op1.contactName_ >= op2);
}


// != operator overloading
bool operator != (const Contact& op1, const Contact& op2) {
	return (*op1.contactName_ != *op2.contactName_);
}

bool operator != (const String& op1, const Contact& op2) {
	return (op1 != *op2.contactName_);
}

bool operator != (const Contact& op1, const String& op2) {
	return (*op1.contactName_ != op2);
}


// == operator overloading
bool operator == (const Contact& op1, const Contact& op2) {
	return (*op1.contactName_ == *op2.contactName_);
}

bool operator == (const String& op1, const Contact& op2) {
	return (op1 == *op2.contactName_);
}

bool operator == (const Contact& op1, const String& op2) {
	return (*op1.contactName_ == op2);
}


// >> operator overloading
istream& operator >> (istream& in, Contact& aContact) {
	String	temp;


	in >> temp;
	if (aContact.contactName_ != nullptr) {
		delete aContact.contactName_;
	}
	aContact.contactName_ = new String(temp);

	return in;	
}


// << operator overloading
ostream& operator << (ostream& out, const Contact& op2) {
	if (op2.contactName_ != nullptr) {
		out << *op2.contactName_;
	}

	return out;
}

// Returns the nth device in the contact's device list
Device* Contact::returnDevice(DeviceNode& head, const int& entryToReturn, int& currentEntry) const {
	Device*	returnPtr = nullptr;

	if (currentEntry < entryToReturn) {
		++currentEntry;

		returnPtr = returnDevice(*head.getNext(), entryToReturn, currentEntry);
	}
	else if (entryToReturn == currentEntry) {
		returnPtr = head.getData();
	}

	return returnPtr;
}

int Contact::interface() {
	int 	successValue = 0;
	char 	inputBuffer = 0;

	String	prompt;
	prompt = "1. Add a device to the device list\n2. Remove a device from the device list\n3. View a device log\nq. Go back to the previous menu.\n\nEnter your choice: "; 

	do {
		successValue = display();
		
		takeInput(inputBuffer, prompt);
		cout << endl;

		switch(inputBuffer) {
			case '1':
				add();
				break;
			case '2':
				remove();
				break;
			case '3':
				viewDevice();
				break;
			case 'q':
				cout << "Returning..." << endl << endl;
				break;
			default:
				cout << "Invalid entry. Please try again..." << endl;
				cout << endl;
				break;
		}
	} while(quitCheck(inputBuffer) != true);

	return successValue;
}


int Contact::add() {
	int 	successValue = 0;
	char 	inputBuffer = 0;

	String	prompt;
	prompt = "What kind of device to add:\n\n1. Add a landline\n2. Add a tablet\n3. Add a smartphone\nq. Go back to the previous menu.\n\nEnter your choice: "; 

	do {
		successValue = display();
		
		takeInput(inputBuffer, prompt);	
		cout << endl;

		switch(inputBuffer) {
			case '1':
				add(landlineType);
				break;
			case '2':
				add(tabletType);
				break;
			case '3':
				add(smartphoneType);
				break;
			case 'q':
				cout << "Returning..." << endl << endl;
				break;
			default:
				cout << "Invalid entry. Please try again..." << endl;
				cout << endl;
				break;
		}
	} while(quitCheck(inputBuffer) != true);

	return successValue;
}


int Contact::add(const DeviceType& aDeviceType) {
	int successValue = 0;
	String	phoneBuffer;
	float	priceBuffer;
	String 	prompt;

	cout << "Enter the new device phone number: ";
	cin >> phoneBuffer;
	cin.ignore(MAX_CHAR, '\n');

	cout << endl;

	prompt = "Enter the monthly price of the new device: ";
	takeInput(priceBuffer, prompt);
	cout << endl;

	if (aDeviceType == landlineType) {
		Landline newDevice(phoneBuffer, priceBuffer);
		Device*	devicePtr = &newDevice;
		successValue = add(head_, devicePtr);
	}
	else if (aDeviceType == tabletType) {
		Tablet newDevice(phoneBuffer, priceBuffer);
		Device*	devicePtr = &newDevice;
		successValue = add(head_, devicePtr);
	}
	else if (aDeviceType == smartphoneType) {
		Smartphone newDevice(phoneBuffer, priceBuffer);
		Device* devicePtr = &newDevice;
		successValue = add(head_, devicePtr);
	}

	return successValue;
}


int Contact::add(const Device* source) {
	return add(head_, source);
}


int Contact::add(DeviceNode*& head, const Device* source) {
	int successValue = 0;

	if (source != nullptr) {
		// We want to add to the list if the head is null
		if (head == nullptr) {
			head = new DeviceNode(source);
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


int Contact::remove() {
	int 	successValue = 1;
	int	inputBuffer = 0;
	String	prompt;
	prompt = "\nEnter the number of the device to remove: "; 

	cout << endl;
	cout << "Remove A Device" << endl;
	cout << "=================" << endl;

	successValue = displayNumbered();

	if (successValue != 0) {
		takeInput(inputBuffer, prompt);
		cout << endl;
	
		if ((inputBuffer <= size_) && (inputBuffer > 0)) {
			int count = 1;

			successValue = remove(head_, inputBuffer, count);

			if (successValue != 0) {
				cout << endl;
				cout << "Success!" << endl << endl;
			}
			else {
				cout << endl;
				cout << "Failed to remove the device from the list";
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

int Contact::remove(DeviceNode*& head, const int& entryToRemove, int& currentEntry) {
	int successValue = 0;

	if (head != nullptr) {
		if (currentEntry < entryToRemove) {
			++currentEntry;

			// We use lookahead here so that our successValue can be communicated
			if (head->getNext() != nullptr) {
				successValue = remove(head->getNext(),
							entryToRemove,
							currentEntry);
			}
		}
		else if (entryToRemove == currentEntry) {
			DeviceNode* hold = head;
			head = head->getNext();

			delete hold;
			--size_;

			successValue = 1;
		}
	}

	return successValue;
}

int Contact::viewDevice() {
	int 	successValue = 1;
	int	inputBuffer = 0;
	String	prompt;
	prompt = "\nEnter the number of the device to view: "; 

	cout << endl;
	cout << "View A Device Log" << endl;
	cout << "===================" << endl << endl;

	successValue = displayNumbered();

	if (successValue != 0) {
		takeInput(inputBuffer, prompt);
	
		if ((inputBuffer <= size_) && (inputBuffer > 0)) {
			int count = 1;

			successValue = viewDevice(head_, inputBuffer, count);

			if (successValue != 0) {
				cout << endl;
				cout << "Success!" << endl << endl;
			}
			else {
				cout << endl;
				cout << "Failed to view the device";
				cout << endl << endl;
			}
		}
		else {
			cout << endl;
			cout << "Invalid entry. Returning to previous menu..." << endl << endl;
		}
	}
	else {
		cout << "Log is empty! Nothing to view. Returning..." << endl << endl;
	}

	return successValue;
}



int Contact::viewDevice(DeviceNode*& head, const int& entryToView, int& currentEntry) {
	int successValue = 0;

	if (head != nullptr) {
		if (currentEntry < entryToView) {
			++currentEntry;

			// We use lookahead here so that our successValue can be communicated
			if (head->getNext() != nullptr) {
				successValue = viewDevice(head->getNext(),
							entryToView,
							currentEntry);
			}
		}
		else if (entryToView == currentEntry) {
			successValue = head->interface();
		}
	}

	return successValue;
}


int Contact::display() const {
	int successValue = 0;

	if (contactName_ != nullptr) {
		cout << *contactName_ << "'s ";
	}
	cout << "Device List:" << endl;
	cout << "===========================================" << endl;

	if (head_ != nullptr) {
		successValue = display(head_);
	}
	else {
		cout << "Device list is empty!" << endl << endl;
	}
	
	return successValue;
}

int Contact::display(const DeviceNode* sourceHead) const {
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

int Contact::displayName() const {
	int successValue = 0;

	if (contactName_ != nullptr) {
		cout << *contactName_;
		successValue = 1;
	}
	
	return successValue;
}

String Contact::getName() const {
	String	returnString;

	if (contactName_ != nullptr) {
		returnString = *contactName_;
	}
	else {
		returnString = "";
	}

	return returnString;
}

int Contact::displayNumbered() const {
	int startNumberingFrom = 1;

	return displayNumbered(head_, startNumberingFrom);
}

int Contact::displayNumbered(const DeviceNode* sourceHead, int& currentCount) const {
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

int Contact::getSize() const {
	return size_;
}


// ContactVertex class
// Default constructor
ContactVertex::ContactVertex() : right_(nullptr), left_(nullptr) {}

// Copy Constructor
ContactVertex::ContactVertex(const ContactVertex& source) : Contact(source), right_(nullptr), left_(nullptr) {}

// Secondary Constructor
ContactVertex::ContactVertex(const String& contactName) : Contact(contactName), right_(nullptr), left_(nullptr) {}

// Destructor
ContactVertex::~ContactVertex() {
	if (right_ != nullptr) {
		right_ = nullptr;
	}
	if (left_ != nullptr) {
		left_ = nullptr;
	}
}

ContactVertex*& ContactVertex::getLeft() {
	return left_;
}

ContactVertex* ContactVertex::getLeft() const {
	return left_;
}

void ContactVertex::setLeft(ContactVertex*& left) {
	left_ = left;
}


ContactVertex*& ContactVertex::getRight() {
	return right_;
}

ContactVertex* ContactVertex::getRight() const {
	return right_;
}

void ContactVertex::setRight(ContactVertex*& right) {
	right_ = right;
}


// ContactBST class
// Default constructor
ContactBST::ContactBST() : root_(nullptr) {}

// Copy constructor
ContactBST::ContactBST(const ContactBST& source) {
	if (source.root_ != nullptr) {
		copyBST(root_, source.root_);
	}
}

// Destructor
ContactBST::~ContactBST() {
	deleteBST(root_);
}

int ContactBST::deleteBST(ContactVertex*& root) {
	int successValue = 0;

	if (root != nullptr) {
		// First go left
		if (root->getLeft() != nullptr) {
			successValue = deleteBST(root->getLeft());
		}
		// Then go right
		if (root->getRight() != nullptr) {
			successValue = deleteBST(root->getRight());
		}
		// Then delete the current vertex
		delete root;
		root = nullptr;
	}

	return successValue;
}

int ContactBST::copyBST(ContactVertex*& destinationRoot, const ContactVertex* sourceRoot) {
	int successValue = 0;

	if (sourceRoot != nullptr) {
		// First deal with the current vertex
		destinationRoot = new ContactVertex(*sourceRoot);
		successValue = 1;
		// Then go left
		if (sourceRoot->getLeft() != nullptr) {
			successValue = copyBST(destinationRoot->getLeft(), sourceRoot->getLeft());
		}
		// Then go right
		if (sourceRoot->getRight() != nullptr) {
			successValue = copyBST(destinationRoot->getRight(), sourceRoot->getRight());
		}
	}

	return successValue;
}

int ContactBST::addContact(const String& newContactName) {
	return addContact(root_, newContactName);
}

int ContactBST::addContact(ContactVertex*& root, const String& newContactName) {
	int successValue = 0;

	if (root != nullptr) {
		// Go left if it it is below 0
		if (newContactName < *root) {
			successValue = addContact(root->getLeft(), newContactName);
		}
		else if (*root == newContactName) {
			// Duplicate contact names are not allowed
			successValue = 0;
		}
		else {
			successValue = addContact(root->getRight(), newContactName);
		}
	}
	else {
		root = new ContactVertex(newContactName);
		successValue = 1;
	}

	return successValue;
}

int ContactBST::display() const {
	int successValue = 0;

	cout << "Contact List" << endl;
	cout << "==============" << endl;

	if (root_ != nullptr) {
		
		successValue = display(root_);
	}
	else {
		cout << "The contact list is empty!" << endl;
		cout << endl;
	}

	return successValue;
}

int ContactBST::display(const ContactVertex* root) const {
	int successValue = 0;

	if (root != nullptr) {
		// First display the left
		if (root->getLeft() != nullptr) {
			successValue = display(root->getLeft());
		}
		// Then display the current
		root->displayName();	
		
		if (root->getSize() > 0) {
			cout << " - " << root->getSize() << " device(s)";
		}
		else {
			cout << "    ----";
		}

		cout << endl;
		successValue = 1;
		// Then display the right
		if (root->getRight() != nullptr) {
			successValue = display(root->getRight());
		}
	}

	return successValue;
}

int ContactBST::deleteFromBST(ContactVertex*& root) {
	int successValue = 0;
	ContactVertex* holdVertex;
	
	if (root != nullptr) {
		// If there are no children
		if ((root->getLeft() == nullptr) && (root->getRight() == nullptr)) {
			delete root;
			root = nullptr;

			successValue = 1;
		}
		// If the node has only one child
		// If there is only a leftChild
		else if ((root->getLeft() != nullptr) && (root->getRight() == nullptr)) {
			holdVertex = root;
			root = root->getLeft();
			delete holdVertex;
			holdVertex = nullptr;

			successValue = 1;
		}
		// If there is only a rightChild
		else if ((root->getRight() != nullptr) && (root->getLeft() == nullptr)) {
			holdVertex = root;
			root = root->getRight();
			delete holdVertex;
			holdVertex = nullptr;

			successValue = 1;
		}
		// If there are two children
		else if ((root->getRight() != nullptr) && (root->getLeft() != nullptr)) {
			// Find the smallest value in the right subtree
			// (inorder successor)

			ContactVertex*	newVertex = nullptr;
			successValue = findDeleteIOS(root->getRight(), newVertex);	

			if (newVertex != nullptr) {
				newVertex->setLeft(root->getLeft());
				newVertex->setRight(root->getRight());
				ContactVertex*	temp = root;
				root = newVertex;
				delete temp;
				temp = nullptr;
			}
		}
	}

	return successValue;
}

// Find the inorder successor (the smallest value in the given subtree)
int ContactBST::findDeleteIOS(ContactVertex*& root, ContactVertex*& IOS) {
	int successValue = 0;

	// Go left until you cant anymore
	// That will be the smallest value in this tree
	if (root->getLeft() != nullptr) {
		successValue = findDeleteIOS(root->getLeft(), IOS);
	}
	else {
		if (IOS == nullptr) {
			IOS = new ContactVertex(*root);
		}
		successValue = deleteFromBST(root);
		root = nullptr;
	}
	return successValue;
}

int ContactBST::removeContact(const String& prompt) {
	String		nameToRemove;
	int 		successValue = 0;

	successValue = display();
	cout << endl;

	if (successValue != 0) {
		cout << prompt;
		cin >> nameToRemove;
		cin.ignore(MAX_CHAR, '\n');
		cout << endl;

		successValue = removeContact(root_,nameToRemove);

		if (successValue != 0) {
			cout << "Success! Returning to the previous menu..." << endl;
			cout << endl;
		}
		else {
			cout << "Name not found. Returning to the previous menu..." << endl;
			cout << endl;
		}

	}
	else {
		cout << "Nothing to remove. Returning..." << endl;
	}

	return successValue;
}

int ContactBST::removeContact(ContactVertex*& root, const String& nameToRemove) {
	int successValue = 0;

	if (root != nullptr) {
		if (nameToRemove == *root) {
			successValue = deleteFromBST(root);
		}
		else if (nameToRemove < *root) {
			successValue = removeContact(root->getLeft(), nameToRemove);
		}
		else {
			successValue = removeContact(root->getRight(), nameToRemove);
		}
	}

	return successValue;
}

Contact* ContactBST::getContactPtr(const String& prompt) {
	String		nameInput;
	int 		successValue = 0;
	Contact*	returnPtr;

	successValue = display();
	cout << endl;

	if (successValue != 0) {
		cout << prompt;
		cin >> nameInput;
		cin.ignore(MAX_CHAR, '\n');
		cout << endl;

		returnPtr = getContactPtr(root_, nameInput);
		if (returnPtr == nullptr) {
			cout << "Contact name not found. Returning to previous menu...";
			cout << endl << endl;
		}
	}
	else {
		cout << "Returning to previous menu..." << endl;
		cout << endl;
	}

	return returnPtr;
}

Contact* ContactBST::getContactPtr(ContactVertex* root, const String& contactName) {
	Contact*	returnPtr = nullptr;

	if (root != nullptr) {
		// Check if we're at the right node
		// If we are, return this contact pointer.
		if (contactName == *root) {
			returnPtr = root;
		}
		// Otherwise, see if we should go left
		else if (contactName < *root) {
			returnPtr = getContactPtr(root->getLeft(), contactName);
		}
		// Otherwise we should go right.
		else {
			returnPtr = getContactPtr(root->getRight(), contactName);
		}
	}

	return returnPtr;
}

Device* ContactBST::getDevicePtr(Contact& sourceContact, const CommType& aCommType, const String& senderOrRecipient) {
	Device*	returnPtr = nullptr;
	int 	successValue = 1;
	int	deviceNumber = 0;
	String	prompt;

	prompt = "\nEnter the number of the ";
	prompt += senderOrRecipient;
	prompt += " device: ";

	cout << endl;
	cout << "Choose a device from ";
	sourceContact.displayName();
	cout << "'s device list: " << endl;
	cout << "====================================================" << endl;

	successValue = sourceContact.displayNumbered();

	if (successValue != 0) {
		takeInput(deviceNumber, prompt);
		cout << endl;
	
		if ((deviceNumber <= sourceContact.getSize()) && (deviceNumber > 0)) {
			Device*	temp = sourceContact[deviceNumber];
			int checkCommType = temp->compareCommType(aCommType);

			if (checkCommType != 0) {	
				returnPtr = temp;
			}
			else {
				cout << "That device can't accept that type of communication. Returning to previous menu..." << endl;
				cout << endl;
			}
		}
		else {
			cout << endl;
			cout << "Invalid entry. Returning to previous menu..." << endl << endl;
		}
	}
	else {
		cout << "Log is empty! No devices available. Returning..." << endl << endl;
	}

	return returnPtr;
}


// CommUI class
// Add a contact
int CommUI::addContact() {
	int 	successValue = 0;
	String	newContactName;

	cout << "Add a contact to the contact list..." << endl;
	cout << "======================================" << endl;
	cout << endl;

	cout << "Enter the name of the new contact: ";
	cin >> newContactName;
	cin.ignore(MAX_CHAR, '\n');
	cout << endl;

	successValue = ContactBST::addContact(newContactName);

	if (successValue != 0) {
		cout << "Success! Opening new contact's interface..." << endl;
		cout << endl;

		Contact* contactPtr = getContactPtr(root_, newContactName);
		if (contactPtr != nullptr) {
			contactPtr->interface();
		}
	}

	return successValue;
}

int CommUI::addContact(const String& newContactName) {
	return ContactBST::addContact(newContactName);
}

int CommUI::removeContact() {
	int 	successValue = 0;
	String	prompt;

	cout << "Remove a contact from the contact list..." << endl;
	cout << "===========================================" << endl;

	prompt = "Enter the name of the contact to remove: ";
	successValue = ContactBST::removeContact(prompt);

	return successValue;
}

int CommUI::goToContact() {
	int 	successValue = 0;
	String	prompt;

	cout << "Add/Remove devices and view/delete communication logs..." << endl;
	cout << "==========================================================" << endl;

	prompt = "Enter the name of the contact to view: ";
	Contact* contactPtr = getContactPtr(prompt);
	if (contactPtr != nullptr) {
		contactPtr->interface();
	}

	return successValue;
}

int CommUI::send() {
	int 		successValue = 0;
	CommType	aCommType;

	Device*		senderDevice = nullptr;
	Contact*	senderContact = nullptr;
	String		senderName;

	Device*		recipientDevice = nullptr;
	Contact*	recipientContact = nullptr;
	String		recipientName;

	String		prompt;
	String		senderOrRecipient;
	int		inputBuffer = 0;
		
	cout << "Send a communication" << endl;
	cout << "========================================" << endl;

	prompt = "Do you want to send a CALL or a TEXT?\n1. Call\n2. Text\n\nEnter the number of your choice: ";
	takeInput(inputBuffer, prompt);
	cout << endl;
	
	if (inputBuffer == 1) {
		aCommType = callType;
	}
	else {
		aCommType = textType;
	}

	// First have them enter the name of a sender
	prompt = "Enter the name of the SENDER contact: ";
	senderContact = getContactPtr(prompt);

	// If they entered a valid Sender contact name
	if (senderContact != nullptr) {
		senderOrRecipient = "sender";
		// Ask them to pick the sender device
		senderDevice = getDevicePtr(*senderContact, aCommType, senderOrRecipient);

		// If they picked a valid sender device
		if (senderDevice != nullptr) {
			// Ask them to enter the name of a recipient
			prompt = "Enter the name of the RECIPIENT contact: ";
			recipientContact = getContactPtr(prompt);

			// If they entered a valid Recipient contact name
			if (recipientContact != nullptr) {
				senderOrRecipient = "recipient";
				// Ask them to pick the recpient device
				recipientDevice = getDevicePtr(*recipientContact, aCommType, senderOrRecipient);

				// If they picked a valid recipient device
				if (recipientDevice != nullptr) {
					// If it was a text take the text message
					if (aCommType == textType) {
						Text newText(aCommType,
							recipientContact->getName(),
							recipientDevice->getPhone(),
							senderContact->getName(),
							senderDevice->getPhone());

						newText.interface();

						// Save the text as a comm pointer
						Communication*	newComm = &newText;

						// Send the text
						senderDevice->send(newComm,
								*recipientDevice);
					}
					// Otherwise it was a call type
					else {
						Connection newCall(aCommType,
							recipientContact->getName(),
							recipientDevice->getPhone(),
							senderContact->getName(),
							senderDevice->getPhone());

						// Save the call as a comm pointer
						Communication*	newComm = &newCall;

						// Send the call
						senderDevice->send(newComm,
								*recipientDevice);
					}
				}
			}
		}
	}

	return successValue;
}



int CommUI::interface() {
	int 	successValue = 0;
	char 	inputBuffer = 0;

	cout << "Contact List Interface" << endl;
	cout << "========================" << endl;
	cout << endl;

	String	prompt;
	prompt = "What would you like to do?\n\n1. Add a contact\n2. Remove a contact\n3. Go to a contact (Add/remove devices and communication logs)\n4. Send a call or a text\nQ. quit\n\nEnter your choice: ";

	do {
		successValue = display();
		cout << endl;

		takeInput(inputBuffer, prompt);	
		cout << endl;

		switch(inputBuffer) {
			case '1':
				addContact();
				break;
			case '2':
				removeContact();
				break;
			case '3':
				goToContact();
				break;
			case '4':
				send();
				break;
			case 'q':
				cout << "Returning..." << endl << endl;
				break;
			default:
				cout << "Invalid entry.  Please try again..." << endl;
				cout << endl;
				break;
		}
	} while(quitCheck(inputBuffer) != true);

	return successValue;
}
