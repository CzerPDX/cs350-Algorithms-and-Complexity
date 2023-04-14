# Weekly Calendar App
The Weekly Calendar App, authored by Brooke Czerwinski, consists of four primary C++ files: `calendar.cpp`, `item.cpp`, `day.cpp`, and `utilities.cpp`. These files define classes and functions for a calendar system that allows users to add, remove, and view daily items such as grocery lists, appointments, and schoolwork. The application uses linked lists, dynamic memory allocation, and provides input validation and error handling utilities.

Brooke Czerwinski authored all the code in this project, and ChatGPT generated this `README.md` based on that code.

## `calendar.cpp`
This file defines a C++ "Calendar" class that uses a doubly linked list to store days. The class provides functions for initialization, manipulation, data loading, and displaying the calendar's contents. The interface() function presents a user menu for adding, removing, and viewing days, navigating months, or exiting the program. The code depends on a separate "Day" class, defined in "day.h".

## `item.cpp`
This file contains classes derived from an abstract `Item` class. `GroceryItem` represents a grocery item with a name and quantity, while `GroceryNode` is a linked list node for `GroceryItem` objects. `GroceryList` manages a linked list of grocery items and provides methods for adding and removing items. The `School` class represents a school class with a name, homework details, and notes, and the `Appointment` class describes an appointment with time, description, and notes. These classes have methods for displaying data and setting values, and virtual destructors for proper memory management.

## `day.cpp`
The `Day` class represents a calendar day and offers methods for adding, displaying, and removing items such as grocery lists, appointments, and schoolwork. It utilizes dynamic memory allocation and includes appropriate destructors to prevent memory leaks. The `ItemNode` class creates a linked list of items within the `Day` class, offering methods for adding, deleting, and modifying items.

## `utilities.cpp`
This file supplies input validation and error handling utilities for various data types. It contains functions for validating integer, float, character, and string inputs, as well as checking for errors and invalid inputs. A function to verify if the user has entered a quit command is also included.

