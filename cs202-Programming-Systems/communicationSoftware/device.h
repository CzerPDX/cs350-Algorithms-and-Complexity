/*
	device.h

	Manages Devices and their derived classes

	Author: Brooke Czerwinski
	Date:	3/1/2020
	Class:	CS202
	Program #3

*/

#include "communication.h"


// Virtual base class that all devices are derived from (landline, tablet, smartphone)
class Device {
public:
	// Default constructor
	Device();
	// Copy constructor
	Device(const Device& source);
	// Constructor from phone number and price
	Device(const String& phoneNumber, const float& price);
	// Destructor
	virtual ~Device();

	// initializes the phoneNumber and price to the argument values
	// Would assist with loading if it was implemented
	/*
		INPUT:	- phoneNumber - String by ref - Phone number to initialize to
				- price - float by ref - price to initialize to
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int createNew(const String& phoneNumber, const float& price);

	// Receive a communication pointer - pure virtual
	// Never intended to be run by this class, but only by derived classes
	virtual int receive(const Communication* dynamicPtr) = 0;

	// Send a communication pointer - pure virtual
	// Never intended to be run by this class, only derived classes
	virtual int send(const Communication* dynamicPtr, Device& recipientDevice) = 0;

	// Displays the device's log interface - pure virtual
	// Never intended to be run by this class, only derived classes
	virtual int interface() = 0;

	// Displays phone number and price. Will be called by derived classes
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	virtual int display() const;

	// Returns a String by value that is the phone number
	// This has a very specific purpose.
	// It's used in the CommUI when creating a new text or connection object.
	// It needs to be returned by value because its must be able to have a lifetime that
	// allows it to be used in the constructor of those objects
	/*
		INPUT:	- NONE
		OUTPUT:	- String by val - Phone number of the device
	*/
	String getPhone() const;

	// Returns a float that is the price of the device per month
	// Also for use in CommUI Text and Connection construction
	/*
		INPUT:	- NONE
		OUTPUT:	- float by val - Price of the device
	*/
	float getPrice() const;

	// Compares the argument CommType to the commtypes of its CommLogs - pure virtual
	// Never meant to be used by this class
	virtual int compareCommType(const CommType& aCommType) const = 0;

private:
	String*	phoneNumber_;	// Phone number of the current device (no formatting)
	float	price_;		// Price per month of the curretn device

	// Releases all dynamic memory
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int releaseMemory();
};


// An interface that manages a comm log and specifies its particular type
// (callType or textType for Connections or Texts)
class CommHandler : public CommLog {
public:
	// Default constructor
	CommHandler();
	// Copy constructor
	CommHandler(const CommHandler& source);

	// Interface for removing calls and texts from the log
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int interface();

	// Receive an incoming communication and activate the proper functions based on
	// the type of communication and the type of CommHandler this is
	// (callType and textType can only be received by callType or textType CommHandlers)
	/*
		INPUT:	- dynamicPtr - Communication ptr - pointer to the incoming Text or Connection (call)
		OUTPUT:	- int - 1 if successful. Adds to CommLog, 0 if failure.
	*/
	int receive(const Communication* dynamicPtr);

	// Sends the incoming Communication to the argument recipient device 
	// through the calling CommHandler
	/*
		INPUT:	- dynamicPtr - Communication ptr - Text or Connection (call) to send
				- recipient Device - Device by ref - Device to receive the Connection
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int send(const Communication* dynamicPtr, Device& recipientDevice);

	// A CommHandler doesn't know its type until its assigned to a device, 
	// which may need more than one CommHandler, so it must be told
	/*
		INPUT:	- aCommType - CommType by ref - CommType to set aCommType_ to
		OUTPUT:	- NONE
	*/
	void setCommType(const CommType& aCommType);

	// Compares argument CommType with its own
	/*
		INPUT:	- source - CommType by ref - CommType to compare against
		OUTPUT:	- int - 1 if the same, 0 if different
	*/
	int compareCommType(const CommType& source) const;

private:
	CommType aCommType_;	// Either callType or textType

	// Receive a call
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int receiveCall();
};



// Landlines are a specific type of device
// That contain a single callType CommHandler
class Landline : public Device {
public:
	// Default constructor
	Landline();
	// Copy constructor
	Landline(const Landline& source);
	// Constructor from phone number and price. ould aid in loading if implemented
	Landline(const String& phoneNumber, const float& price);
	// Destructor
	~Landline();

	// Opens interface to display landline call log options
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int interface();

	// Displays the details of the landline device
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display() const;

	// Receive an incoming communication
	// Will fail if the communication is not a callType
	/*
		INPUT:	- dynamicPtr - Communication ptr - An incoming communication to process
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int receive(const Communication* dynamicPtr);

	// Compares landline CommType (callType) with argument CommType
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if it is the same, 0 if different.
	*/
	int compareCommType(const CommType& source) const;

	// Sends the argument Communication from the landline to the recipientDevice
	/*
		INPUT:	- dynamicPtr - Communication ptr - outgoing Connection 
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int send(const Communication* dynamicPtr, Device& recipientDevice);

private:
	CommHandler*	aCallHandler_;	// Handles callType Communications for the landline
};


// Tablets are a specific type of device. That contain a single textType CommHandler
class Tablet : public Device {
public:
	// Default constructor
	Tablet();
	// Copy constructor
	Tablet(const Tablet& source);
	// Constructor from phone number and price
	Tablet(const String& phoneNumber, const float& price);
	// Destructor
	~Tablet();

	// Opens interface to display tablet text log options
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int interface();

	// Displays the details of the Tablet device
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display() const;

	// Receive an incoming communication
	// Will fail if the communication is not a textType
	/*
		INPUT:	- dynamicPtr - Communication ptr - An incoming communication to process
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int receive(const Communication* dynamicPtr);

	// Compares tablet CommType (textType) with argument CommType
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if it is the same, 0 if different.
	*/
	int compareCommType(const CommType& aCommType) const;

	// Sends the argument Communication from the tablet to the recipientDevice
	/*
		INPUT:	- dynamicPtr - Communication ptr - outgoing Text
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int send(const Communication* dynamicPtr, Device& recipientDevice);

private:
	CommHandler*	aTextHandler_;	// Handles all texts to smartphone
};


// Landlines are a specific type of device
// That contain both a callType and a separate textType CommHandler
class Smartphone : public Device {
public:
	// Default constructor
	Smartphone();
	// Copy constructor
	Smartphone(const Smartphone& source);
	// Constructor from phone number and price
	Smartphone(const String& phoneNumber, const float& price);
	// Destructor
	~Smartphone();

	// Opens interface to display smartphone call and text log options
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int interface();

	// Displays the details of the smartphone device
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int display() const;

	// Receive an incoming communication. Will fail if the communication is not a callType OR a textType
	/*
		INPUT:	- dynamicPtr - Communication ptr - An incoming communication to process
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int receive(const Communication* dynamicPtr);

	// Compares landline CommType (callType and textType) with argument CommType
	/*
		INPUT:	- NONE
		OUTPUT:	- int - 1 if it is the same, 0 if different.
	*/
	int compareCommType(const CommType& source) const;

	// Sends the argument Communication from the smartphone to the recipientDevice
	/*
		INPUT:	- dynamicPtr - Communication ptr - outgoing Text or Connection
		OUTPUT:	- int - 1 if successful, 0 if failure.
	*/
	int send(const Communication* dynamicPtr, Device& recipientDevice);

private:
	CommHandler*	aCallHandler_;	// Handles all calls to smartphone
	CommHandler*	aTextHandler_;	// Handles all texts to smartphone
};
