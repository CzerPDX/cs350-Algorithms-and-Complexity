/*
	device.cpp

	Manages Devices and their derived classes

	Author: Brooke Czerwinski
	Date:	3/1/2020
	Class:	CS202
	Program #3

*/

#include "device.h"

using std::cout;
using std::endl;
using std::cin;


// Device class
// Default constructor
Device::Device() : phoneNumber_(nullptr), price_(0) {}
// Copy constructor
Device::Device(const Device& source) : phoneNumber_(nullptr), price_(source.price_) {
	if (source.phoneNumber_ != nullptr) {
		phoneNumber_ = new String(*source.phoneNumber_);
	}
}

// Secondary constructor
Device::Device(const String& phoneNumber, const float& price) {
	phoneNumber_ = new String(phoneNumber);
	price_ = price;
}

// Destructor
Device::~Device() {
	releaseMemory();
}

int Device::releaseMemory() {
	int successValue = 0;

	if (phoneNumber_ != nullptr) {
		delete phoneNumber_;
		phoneNumber_ = nullptr;
	}
	price_ = 0;
	if (phoneNumber_ == nullptr) {
		successValue = 1;
	}

	return successValue;
}

int Device::createNew(const String& phoneNumber, const float& price) {
	int successValue = 0;

	successValue = releaseMemory();
	phoneNumber_ = new String(phoneNumber);
	price_ = price;

	return successValue;
}

int Device::display() const {
	int successValue;

	if (phoneNumber_ != nullptr) {
		cout << "Phone number: " << *phoneNumber_ << endl;
		cout << "Price per month: $" << printf("%.1f", price_) << endl;
		successValue = 1;
	}

	return successValue;
}

// This is returned by reference just in case phoneNumber_ is null
// We must return phoneNumber because it must be given to the Communication Node
// when a text or call is sent or received by these devices.
// We have the Communication hierarchy keep track of the phone numbers because
// that is the packet of information being sent to/from, but must be gotten from
// the device list (here) within the contact list.
String Device::getPhone() const {
	if (phoneNumber_ != nullptr) {
		return *phoneNumber_;
	}
	else {
		String empty;
		empty = "";
		return empty;
	}
}

float Device::getPrice() const {
	return price_;
}


// Device class
// Default constructor
CommHandler::CommHandler() : aCommType_(callType) {}
// Copy constructor
CommHandler::CommHandler(const CommHandler& source) : CommLog(source), aCommType_(source.aCommType_) {}

// Receive a communication
int CommHandler::receive(const Communication* dynamicPtr) {
	int successValue = 0;
	// Only do anything if the pointer is the right type
	if (dynamicPtr->compareCommType(aCommType_) == 1) {
		// Add to CommLog
		successValue = add(dynamicPtr);
		// Check to see if it's a call. If it is, activate the receive call function
		const Text* textPtr = dynamic_cast<const Text*>(dynamicPtr);

		if (textPtr == nullptr) {
			receiveCall();
		}
	}

	return successValue;
}

// Comm handler interface
int CommHandler::interface() {
	if (aCommType_ == callType) {
		cout << "Call Log" << endl;
		cout << "==========" << endl;
	}
	else {
		cout << "Text Log" << endl;
		cout << "==========" << endl;
	}
	return CommLog::interface();
}

// Receive a phone call
int CommHandler::receiveCall() {
	int 	successValue = 0;
	char	input[MAX_CHAR];
	String	prompt;

	cout << "Call established..." << endl << endl;
	cout << "Press enter to end the call: " << endl;
	cin.getline(input, MAX_CHAR, '\n');
	successValue = 1;

	return successValue;
}

int CommHandler::send(const Communication* dynamicPtr, Device& recipientDevice) {
	int successValue = 0;

	// Only do anything if the pointer is the right type
	if (dynamicPtr->compareCommType(aCommType_) == 1) {
		int successValue = recipientDevice.receive(dynamicPtr);
		// Add to CommLog
		if (successValue != 0) {
			successValue = add(dynamicPtr);
		}
	}

	return successValue;
}

void CommHandler::setCommType(const CommType& aCommType) {
	aCommType_ = aCommType;
}

int CommHandler::compareCommType(const CommType& aCommType) const {
	return (aCommType_ == aCommType);
}


// Landline class
// Default constructor
Landline::Landline() : aCallHandler_(nullptr) {
	// Allocate a call handler for the device and set its comm type
	aCallHandler_ = new CommHandler;
	// This call handler will be used for phone calls, so set its CommType to callType
	CommType aCommType = callType;
	aCallHandler_->setCommType(aCommType);
}

// Copy constructor
Landline::Landline(const Landline& source) : Device(source), 
						aCallHandler_(nullptr)  {
	if (source.aCallHandler_ != nullptr) {
		aCallHandler_ = new CommHandler(*source.aCallHandler_);
	}
}

// Secondary construtor
Landline::Landline(const String& phoneNumber, const float& price) :
				Device(phoneNumber, price),
				aCallHandler_(nullptr) {
	aCallHandler_ = new CommHandler();

	CommType aCommType = callType;
	aCallHandler_->setCommType(aCommType);
}

Landline::~Landline() {
	if (aCallHandler_ != nullptr) {
		delete aCallHandler_;
		aCallHandler_ = nullptr;
	}
}

int Landline::interface() {
	return aCallHandler_->interface();
}

int Landline::display() const {	
	int successValue = 0;

	cout << "Landline" << endl;
	cout << "==========" << endl;
	successValue = Device::display();
	cout <<  "Can send/receive calls" << endl << endl;

	return successValue;
}

int Landline::receive(const Communication* dynamicPtr) {
	int successValue = 0;
	successValue = aCallHandler_->receive(dynamicPtr);

	if (successValue == 0) {
		cout << "Recipient device cannot accept ";
		CommType temp = textType;
		if(dynamicPtr->compareCommType(temp) == 1) {
			cout << "text messages.";
		}
		else {
			cout << "phone calls.";
		}

		cout << endl;
	}

	return successValue;
}

int Landline::send(const Communication* dynamicPtr, Device& recipientDevice) {
	int successValue = 0;

	if (aCallHandler_ != nullptr) {
		successValue = aCallHandler_->send(dynamicPtr, recipientDevice);
	}

	return successValue;
}

int Landline::compareCommType(const CommType& aCommType) const {
	int successValue = 0;
	if (aCallHandler_ != nullptr) {
		successValue = aCallHandler_->compareCommType(aCommType);
	}

	return successValue;
}


// Tablet class
// Default constructor
Tablet::Tablet() : aTextHandler_(nullptr) {
	CommType aCommType = textType;
	aTextHandler_->setCommType(aCommType);
}

// Copy constructor
Tablet::Tablet(const Tablet& source) : Device(source), 
						aTextHandler_(nullptr) {
	if (source.aTextHandler_ != nullptr) {
		aTextHandler_ = new CommHandler(*source.aTextHandler_);
	}
}

// Secondary construtor
Tablet::Tablet(const String& phoneNumber, const float& price) : Device(phoneNumber, price), aTextHandler_(nullptr) {
	aTextHandler_ = new CommHandler();

	CommType aCommType = textType;
	aTextHandler_->setCommType(aCommType);
}

Tablet::~Tablet() {
	if (aTextHandler_ != nullptr) {
		delete aTextHandler_;
		aTextHandler_ = nullptr;
	}
}

int Tablet::interface() {
	return aTextHandler_->interface();
}

int Tablet::display() const {
	int successValue = 0;
	cout << "Tablet" << endl;
	cout << "========" << endl;
	successValue = Device::display();
	cout <<  "Can send/receive text messages" << endl << endl;

	return successValue;
}

int Tablet::receive(const Communication* dynamicPtr) {
	int successValue = 0;
	if (aTextHandler_ == nullptr) {
		aTextHandler_ = new CommHandler;
	}
	successValue = aTextHandler_->receive(dynamicPtr);

	if (successValue == 0) {
		cout << "Recipient device cannot accept ";
		CommType temp = textType;
		if(dynamicPtr->compareCommType(temp) == 1) {
			cout << "text messages.";
		}
		else {
			cout << "phone calls.";
		}
		cout << endl;
	}

	return successValue;
}

int Tablet::send(const Communication* dynamicPtr, Device& recipientDevice) {
	int successValue = 0;
	if (aTextHandler_ != nullptr) {
		successValue = aTextHandler_->send(dynamicPtr, recipientDevice);
	}

	return successValue;
}

int Tablet::compareCommType(const CommType& aCommType) const {
	int successValue = 0;
	if (aTextHandler_ != nullptr) {
		successValue = aTextHandler_->compareCommType(aCommType);
	}

	return successValue;
}


// Smartphone class
// Default constructor
Smartphone::Smartphone() : aCallHandler_(nullptr), aTextHandler_(nullptr) {
	// Allocate a call handler for the device and set its comm type
	aCallHandler_ = new CommHandler;

	// This call handler will be used for phone calls, so set its CommType to callType
	CommType aCommType = callType;
	aCallHandler_->setCommType(aCommType);

	// Allocate a text handler for the device and set its comm type
	aTextHandler_ = new CommHandler;

	// This call handler will be used for text messages, so set its CommType to textType
	aCommType = textType;
	aTextHandler_->setCommType(aCommType);
}

// Copy constructor
Smartphone::Smartphone(const Smartphone& source) : Device(source), 
						aCallHandler_(nullptr),
						aTextHandler_(nullptr) {
	// If the incoming smartphone has a call handler copy it
	if (source.aCallHandler_ != nullptr) {
		aCallHandler_ = new CommHandler(*source.aCallHandler_);
	}
	// Otherwise create an empty one and set its commType to call
	// I think this would never happen but is included for safety!
	else {
		aCallHandler_ = new CommHandler;
		CommType aCommType = callType;
		aCallHandler_->setCommType(aCommType);
	}

	// If the incoming smartphone has a text and handler copy it	
	if (source.aTextHandler_ != nullptr) {
		aTextHandler_ = new CommHandler(*source.aTextHandler_);
	}
	// Otherwise create an empty one and set its commType to text
	else {
		aTextHandler_ = new CommHandler;
		CommType aCommType = textType;
		aTextHandler_->setCommType(aCommType);
	}
}

// Secondary construtor
Smartphone::Smartphone(const String& phoneNumber, const float& price) :
				Device(phoneNumber, price),
				aCallHandler_(nullptr),
				aTextHandler_(nullptr) {
	aCallHandler_ = new CommHandler();
	CommType aCommType = callType;
	aCallHandler_->setCommType(aCommType);

	aTextHandler_ = new CommHandler();
	aCommType = textType;
	aTextHandler_->setCommType(aCommType);
}

Smartphone::~Smartphone() {
	if (aCallHandler_ != nullptr) {
		delete aCallHandler_;
		aCallHandler_ = nullptr;
	}

	if (aTextHandler_ != nullptr) {
		delete aTextHandler_;
		aTextHandler_ = nullptr;
	}
}

int Smartphone::interface() {
	int 	successValue = 0;
	char 	inputBuffer = 0;
	String	prompt;
	prompt = "1. Call Log\n2. Text Log\nq. Go back to the previous menu.\n\nEnter your choice: "; 

	do {
		cout << "Smartphone Interface" << endl;
		cout << "======================" << endl;
		takeInput(inputBuffer, prompt);
		cout << endl;

		switch(inputBuffer) {
			case '1':
				aCallHandler_->interface();
				break;
			case '2':
				aTextHandler_->interface();
				break;
			case 'q':
				cout << "Returning..." << endl << endl;
				break;
			default:
				cout << "Invalid entry. Please try again..." << endl;
				cout << endl;
				break;
		}

		successValue = 1;
	} while(quitCheck(inputBuffer) != true);

	return successValue;
}

int Smartphone::display() const  {
	int successValue = 0;
	cout << "Smartphone" << endl;
	cout << "============" << endl;
	successValue = Device::display();
	cout <<  "Can send/receive calls and text messages" << endl << endl;

	return successValue;
}

int Smartphone::receive(const Communication* dynamicPtr) {
	int successValue = 0;
	// First try to send the communication to the call handler
	// This will fail (return 0) if the communication is not of a call type
	successValue = aCallHandler_->receive(dynamicPtr);

	// If that doesn't work
	if (successValue == 0) {
		// Try to send the communcation to the text handler
		// This will return 0 if the communication is not of a text type
		successValue = aTextHandler_->receive(dynamicPtr);

		// If neither handler accepted the communication
		if (successValue == 0) {
			cout << "Recipient device can't accept communications of that type.";
			cout << endl;
		}
	}

	return successValue;
}

int Smartphone::send(const Communication* dynamicPtr, Device& recipientDevice) {
	int successValue = 0;

	if (dynamicPtr != nullptr) {
		if ((dynamicPtr->compareCommType(callType) == 1) && 
						(aCallHandler_ != nullptr)) {
			successValue = aCallHandler_->send(dynamicPtr, recipientDevice);
		}
		else if ((dynamicPtr->compareCommType(textType) == 1) && 
						(aTextHandler_ != nullptr)) {
			successValue = aTextHandler_->send(dynamicPtr, recipientDevice);
		}
	}

	return successValue;
}

int Smartphone::compareCommType(const CommType& aCommType) const {
	int successValue = 0;

	if (aCallHandler_ != nullptr) {
		successValue = aCallHandler_->compareCommType(aCommType);
	}
	if ((successValue != 1) && (aTextHandler_ != nullptr)) {
		successValue = aTextHandler_->compareCommType(aCommType);
	}

	return successValue;
}
