/*
    Brooke Czerwinski
    CS 415 Parallel Programming
    Assignment 1

    prodcons2.cpp

    C++ program to use parallel thread to add to and remove from a queue.
    Takes an int argument from command line and generates that many producer threads

    References used:
    https://en.cppreference.com/w/cpp/thread/condition_variable
    https://man7.org/linux/man-pages/man3/sched_getcpu.3.html
    https://www.cplusplus.com/reference/condition_variable/condition_variable/
    https://www.cplusplus.com/reference/condition_variable/condition_variable/wait/ 
*/
#include "queue.h"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <sched.h>
#include <thread>

using std::cerr;
using std::condition_variable;
using std::cout;
using std::endl;
using std::mutex;
using std::stoi;
using std::thread;
using std::unique_lock;


// Required Global Parameters
int     BUFSIZE = 20;       // queue capacity
int     NUMITEMS = 100;     // total number of data items

// Other Global Parameters
Queue               *aQueue;            // Queue pointers
int                 *consumerOutputArr;
mutex               mtx;                // Mutex variable
mutex               outputMtx;          // Mutex variable for consumer counts
condition_variable  cv;                 // Condition Variable
condition_variable  cv2;
bool                addToQueue = true;
bool                removeFromQueue = false;
bool                ioReady = false;
bool                outputArrReady = false;

// Function prototypes
void producer(int k);
void consumer(int k);                 
long checkInput(const int argc, const char input[]);     // Returns valid input or 0 if failed

int main(int argc, char *argv[])
{
    bool    errorFlag = false;
    long    numCons = 0;
    int     total = 0;
    
    // checkInput returns valid argument as an integer
    if (argc <= 2)
    {
        if (argc == 1)
        {
            numCons = 1;
        }
        else
        {
            numCons = checkInput(argc, argv[1]);
        }

        if (numCons == 0)
        {
            numCons = 1;
        }
        
        // Initialize queue
        aQueue = new Queue(BUFSIZE);

        // Initialize consumerOutputArr
        consumerOutputArr = new int[numCons];

        // Create producer thread
        thread  producerThread(producer, numCons);
        // Create consumer threads
        thread  consumerThreads[numCons];   // An array of consumer threads
        for (int i = 0; i < numCons; i++)
        {
            consumerThreads[i] = thread(consumer, i);
        }

        // Set the ioReady to true and notify one of the threads to start
        ioReady = true;
        cv.notify_one();
        outputArrReady = true;
        cv2.notify_one();

        // Join producer threads
        producerThread.join();
        // Join consumer threads
        for (int i = 0; i < numCons; i++)
        {
            consumerThreads[i].join();
        }

        // Final message
        cout << endl;
        cout << "Main: all done!" << endl;
        cout << "Consumer stats: [";

        for (int i = 0; i < numCons; i++)
        {
            cout << consumerOutputArr[i] << ",";
            total += consumerOutputArr[i];
        }
        cout << "] total = " << total << endl;
    
        // Release memory
        delete aQueue;
    }
    else
    {
        cerr << "Error, bad arguments. Please provide a single integer for the number of producer threads." << endl;
    }
    
    return 0;
}


// Checks to make sure program argument is a single valid integer
long checkInput(const int argc, const char input[])
{
    long numCons = 0;

    if (argc == 2)
    {
        numCons = strtol(input, NULL, 0);
    }

    return numCons;
}



void producer(int k)
{
    // Uses curly braces to establish a new scope so that "lck" will get 
    // destroyed (and lock released) when it goes out of scope.
    {
        // Obtain a lock on the i/o so messages don't get mixed together
        /*
            The lock is created using the mtx variable
            It will wait until getting a wakeup signal from the condition variable (cv)
        */
        unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, []{return ioReady;});
        // Set the ioReady to false. Prevents competing i/o in case of spurrious wakeup
        ioReady = false;
        cout << "Producer starting on core " << sched_getcpu() << endl;
        // Set the ioReady variable to true to set it up for the next waiting thread that gets to run
        ioReady = true;
        // Send a wakeup signal to one of the threads waiting using the condition variable (cv)
        cv.notify_one();
    }
 
    for (int i = 1; i <= NUMITEMS; i++)
    {
        // Obtain a lock based on the mtx variable.
        unique_lock<std::mutex> lck(mtx);
        // Wait until the condition variable sends a signal
        /*
            Notes for self:
            From https://www.cplusplus.com/reference/condition_variable/condition_variable/wait/

            If pred is specified (2), the function only blocks if pred returns false, 
            and notifications can only unblock the thread when it becomes true (which 
            is specially useful to check against spurious wake-up calls). 
            This version (2) behaves as if implemented as:
                    while (!pred()) wait(lck);
        */
        cv.wait(lck, []{return addToQueue;});

        cout << "Producer added " << i << " (qsz: " << aQueue->size() << ")" << endl;     
        // Add to the queue   
        aQueue->add(i);

        // If the queue has at least 1 item in it, set removeFromQueue to true
        if (aQueue->size() == 1)
        {
            removeFromQueue = true;
        }
        // If the queue is full, set addToQueue to false
        else if (aQueue->size() == BUFSIZE)
        {
            addToQueue = false;
        }
        // Send a wakeup signal to one of the threads waiting using the condition variable (cv)
        cv.notify_one();
    }

    // Add k worth of -1's to the end of the queue
    for (int i = 0; i < k; i++)
    {
        unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, []{return addToQueue;});
        aQueue->add(-1);

        // If the queue has at least 1 item in it, set removeFromQueue to true
        if (aQueue->size() == 1)
        {
            removeFromQueue = true;
        }
        // If the queue is full, set addToQueue to false
        else if (aQueue->size() == BUFSIZE)
        {
            addToQueue = false;
        }
        // Send a wakeup signal to one of the threads waiting using the condition variable (cv)
        cv.notify_one();
    }

    {
        // Obtain a lock based on the mtx variable.
        unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, []{return ioReady;});
        // Set the ioReady to false. Prevents competing i/o in case of spurrious wakeup
        ioReady = false;
        cout << "Producer ending" << endl;
        // Set the ioReady variable to true to set it up for the next waiting thread that gets to run
        ioReady = true;
        // Send a wakeup signal to one of the threads waiting using the condition variable (cv)
        cv.notify_one();
    }
    
}



void consumer(int k)
{
    int totalRemoved = 0;
    int lastRemoved = 0;

    // Uses empty curly braces so that the unique lock will
    // die as it goes out of scope.
    // Lock the mutex so that the 
    {
        // Obtain a lock based on the mtx variable.
        unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, []{return ioReady;});
        // Set the ioReady to false. Prevents competing i/o in case of spurrious wakeup
        ioReady = false;
        cout << "Consumer [" << k << "] starting on core " << sched_getcpu() << endl;
        // Set the ioReady variable to true to set it up for the next waiting thread that gets to run
        ioReady = true;
        // Send a wakeup signal to one of the threads waiting using the condition variable (cv)
        cv.notify_one();
    }

    do
    {
        // Obtain a lock based on the mtx variable.
        unique_lock<std::mutex> lck(mtx);

        // wait for the condition variable to send a wakeup signal to continue
        // Secondary check: removeFromQueue must be true
        cv.wait(lck, []{return removeFromQueue;});

        // Output mesage and remove from queue at the same time
        lastRemoved = aQueue->remove();
        cout << "Consumer [" << k << "] rem'd " << lastRemoved << " (qsz: " << aQueue->size() << ")" << endl;

        // If queue is empty set removeFromQueue to false
        if (aQueue->size() == 0)
        {
            removeFromQueue = false;
        }
        // If queue has at least one remaining space in it, set addToQueue to true.
        else if (aQueue->size() == (BUFSIZE - 1))
        {
            addToQueue = true;
        }

        // Send a wakeup signal to one of the threads waiting using the condition variable (cv)
        cv.notify_one();

        if (lastRemoved != -1)
        {
            totalRemoved++;
        }
    } while (lastRemoved != -1);

    {
        unique_lock<std::mutex> lck(outputMtx);
        cv2.wait(lck, []{return outputArrReady;});
        outputArrReady = false;
        consumerOutputArr[k] = totalRemoved;
        outputArrReady = true;
        cv2.notify_one();
    }

    {
        unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, []{return ioReady;});
        // Set the ioReady to false. Prevents competing i/o in case of spurrious wakeup
        ioReady = false;
        cout << "Consumer [" << k << "] ending" << endl;
        // Set the ioReady variable to true to set it up for the next waiting thread that gets to run
        ioReady = true;
        // Send a wakeup signal to one of the threads waiting using the condition variable (cv)
        cv.notify_one();
    }
}