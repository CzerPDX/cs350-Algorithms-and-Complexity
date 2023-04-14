# Communication App for CS202 Programming Systems
Written in C++ in 2020.

Mimics the workings of a communication system.

This software allows you to:
- Add a new contact to your contacts list.
- Assign different devices to those contacts (e.g., landline, tablet, or smartphone). Each of these devices is capable of receiving different types of communications.
    - Landline: phone calls only
    - Tablets: text messages only
    - Smartphone: both phone calls and text messages
- Use that communication method to send a fake message or make a fake call.
- Log the record of your communications.
- View the details of your account, including your lines and communication log.

The assignment focuses on inheritance and operator overloading. I was not allowed to use the built-in string class but was allowed to write my own. This custom string class can be found in `utilities.cpp`.

Some parts of this README.md were generated using ChatGPT, but all code was written by Brooke Czerwinski.

# communication.cpp
`communication.cpp` is responsible for managing the interactions and communication logs between devices within a communication application. The file contains various classes that handle the creation and organization of different communication types, such as calls and texts, along with the management of communication logs using object-oriented programming techniques and data structures like linked lists.

### `Communication` class
This is a virtual class that serves as the base class for the `Connection` and `Text` classes.

### `Connection` class
The `Connection` class manages basic connections and is essentially the "call" class. It inherits from the `Communication` class and has a default constructor, copy constructor, constructor with arguments, and a destructor. It displays the recipient and sender names and phone numbers, and compares communication types. It has a `CommType` object for text or connection, and `String` objects for the recipient and sender's names and phone numbers. It also has a private method for releasing all dynamic memory.

### `Text` class
The `Text` class is responsible for managing text messages. It is a derived class of `Connection`. It has several constructors, including one that takes a `Connection` object and a `String` object as input. It also has methods for displaying and requesting input for the text message, as well as releasing all dynamic memory.

### `CommNode` class
The `CommNode` class manages a node of a linked list that stores communication data. It has functions to get and set the communication data and the next node in the list, and to display the contents of the communication data. It also has functions to copy the data from an argument communication object and to release dynamic memory.

### `CommLog` class
The `CommLog` class represents a linked list of communication pointers with dynamic binding for texts or calls. It offers functionality to add new communication, display the communication log, remove a communication entry by its position number, and navigate a menu interface. The class methods use recursion to achieve the desired functionality.

# contact.cpp
`contact.cpp` is responsible for managing the contact information and associated devices within a communication application. The file contains various classes that handle the creation, organization, and navigation of contacts, their devices, and the user interface for managing these components in a binary search tree data structure.

### `DeviceNode` class
The `DeviceNode` class represents a device node for use in a device LLL (linear linked list). It has member functions for receiving incoming communication to the device, calling the device's interface, displaying the device's details, setting the next pointer of the node, and getting the device data pointer. It also has constructors, a destructor, and a copy function for the device data.

### `Contact` class
The `Contact` class represents a contact and has member functions for adding devices to the contact's device list, displaying the contact's name and devices,and navigating through the list to view or remove a device. It also has overloaded operators for assignment, addition, subscripting, and relational/equality comparisons, as well as input/output operators. The class has constructors, a destructor, and functions for copying and releasing memory, displaying the contact's info and devices, and adding a device of a specific type.

### `ContactVertex` class
The `ContactVertex` class is used by the `ContactBST` class and inherits from the `Contact` class. It has member functions for getting and setting the left and right pointers of the vertex, and constructors, a destructor, and a copy function. The class represents a vertex in a binary search tree of contacts.

### `ContactBST` class
The `ContactBST` class is a binary search tree implementation for storing `ContactVertex` objects, which are derived from a `Contact` class. It has several member functions to add contacts, remove contacts, get contact pointers, get device pointers from a contact, and display the contact list. It also includes member variables such as the root of the BST structure. Additionally, it includes a senderRecipient enum for defining sender and recipient communication types.

### `CommUI` class
The CommUI class is responsible for managing the final level interface and creating communications. It inherits from ContactBST and contains methods for adding and removing contacts, going to a specific contact, and sending communications. The interface() method launches the main interface.

# device.cpp
`device.cpp` is responsible for handling different types of devices within a communication application. The file contains various classes that manage the creation, functionality, and interactions of devices such as landlines, tablets, and smartphones, as well as their communication logs and interfaces.

### `Device` class
The `Device` class is a virtual base class that all devices (e.g., landline, smartphone, tablet) are derived from. It has methods for initializing a new device, receiving and sending communication, displaying the device's log interface, and comparing the `CommType` to the `CommLogs`. It also has methods for displaying the phone number and price of the device, as well as for getting the phone number and price, which are used in the `CommUI` when creating new text or connection objects.

### `CommHandler` class
The `CommHandler` class is an interface that manages a communication log and specifies its particular type as either a `callType` or `textType` for connections or texts. It inherits from the `CommLog` class and provides additional functionalities such as receiving and sending communications, comparing communication types, and setting communication types. It also provides a specialized `receiveCall()` function to receive calls.

### `Landline` class
The `Landline` class is a type of `Device` and contains a single `CommHandler` object that handles callType communications. It has methods to display device details, open an interface to display landline call log options, receive incoming callType communications, compare its CommType with an argument CommType, and send outgoing callType communications to a recipient device.

### `Tablet` class
The `Tablet` class is a specific type of device that contains a single textType `CommHandler`. It has functions that open an interface to display tablet text log options, display the details of the Tablet device, receive an incoming communication, compare tablet CommType (textType) with argument CommType, and send the argument Communication from the tablet to the recipientDevice.

### `Smartphone` class
The `Smartphone` class is a specific type of device that includes both a callType and a separate textType `CommHandler`. The class includes methods to open an interface to display the device's call and text logs, display device details, receive an incoming communication, compare the `CommType` of the device with an argument, and send a communication to a recipient device.

# utilities.cpp
`utilities.cpp` is responsible for providing utility functions and a custom string implementation in a communication application. The file contains a custom `String` class for string manipulation and various utility functions that assist in user input validation, error checking, and string operations, enhancing the overall functionality and user experience.

### `String` class
The `String` class is a custom implementation of a string data structure with methods for creating, copying, assigning, and manipulating strings. It provides functionality for concatenation, comparison, and indexing, among others. This implementation uses a C-style string internally, and provides a simpler interface to use it. All standard operator overloading is used.

### Various utilities
- `takeInput()`: Validates user input of integers, floats, characters, and cstrings, and prompts the user to enter a new input in case of an invalid one.
- `cinErrorCheck()`: Checks if there is a cin error and sets a flag accordingly.
- `checkNext()`: Checks if there is anything left in the input buffer.
- `invalidEntry()`: Prints an error message when there is an invalid entry.
- `quitCheck()`: Checks if the user input is "quit" or not.
- `strcmpCI()`: Compares two cstrings case-insensitively.
- `toLowercase()`: Changes a cstring to all lowercase.

# Summarized Software Requirements 
This programming assignment focuses on operator overloading and inheritance in an object-oriented environment. The goal is to correctly create classes that properly support the copy constructor, destructor, and assignment operator when dynamic memory is involved. The program aims to apply the functionality of the operators and appropriate arguments and residual values for the operators to solve a real-world problem related to cell phones. The program should be designed incrementally, focusing on the problem before applying the data structure. The derived and base class should manage dynamic memory to experience the full breadth of the operators.

- The program should support operator overloading and inheritance.
- The primary goal is to experience operator overloading in an object-oriented environment.
- Classes that manage dynamic memory must have a copy constructor, destructor, and assignment operator.
- The program should apply the functionality of operators and appropriate arguments and residual values to solve a real-world problem related to cell phones.
- Operators should be applied in hierarchies since the program explores OOP.
The residual value of the operator should be considered, and the returned type used.
- Memory of the residual value should be controlled by either the client program or the operator.
- Operands should be constant arguments if not modified.
- Member functions or friends can be used for operator overloading.
- The program should be designed incrementally, focusing on the problem before applying the data structure.
- The derived and base class should manage dynamic memory to experience the full breadth of the operators.
