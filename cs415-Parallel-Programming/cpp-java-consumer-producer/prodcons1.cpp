/*
    Brooke Czerwinski
    CS 415 Parallel Programming
    Assignment 1

    prodcons1.cpp

    C++ program to use parallel thread to add to and remove from a queue.

    References used:
    https://en.cppreference.com/w/cpp/thread/condition_variable
    https://man7.org/linux/man-pages/man3/sched_getcpu.3.html
    https://www.cplusplus.com/reference/condition_variable/condition_variable/
    https://www.cplusplus.com/reference/condition_variable/condition_variable/wait/
    https://www.baeldung.com/java-static
*/

#include "queue.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sched.h>

using std::cin;
using std::cout;
using std::endl;
using std::mutex;
using std::thread;
using std::condition_variable;
using std::unique_lock;



// Required Global Parameters
int     BUFSIZE = 20;       // queue capacity
int     NUMITEMS = 100;     // total number of data items

// Other Global Parameters
Queue               *aQueue;    // Queue pointers
mutex               mtx;          // Mutex variable
mutex               iosMtx;
condition_variable  cv;         // Condition Variable
bool                addToQueue = true;
bool                removeFromQueue = false;

bool                ioReady = false;


// Function prototypes
void producer();
void consumer();

int main()
{
    // Initialize queue
    aQueue = new Queue(BUFSIZE);

    // Create threads
    thread  producerThread(producer);
    thread  consumerThread(consumer);

    
    ioReady = true;
    cv.notify_one();


    // Join threads
    producerThread.join();
    consumerThread.join();

    // Final message
    cout << "Main: all done!" << endl;
    
    // Release memory
    delete aQueue;

    return 0;
}

void producer()
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

void consumer()
{
    // Uses empty curly braces so that the unique lock will
    // die as it goes out of scope.
    // Lock the mutex so that the 
    {
        // Obtain a lock based on the mtx variable.
        unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, []{return ioReady;});
        // Set the ioReady to false. Prevents competing i/o in case of spurrious wakeup
        ioReady = false;
        cout << "Consumer starting on core " << sched_getcpu() << endl;
        // Set the ioReady variable to true to set it up for the next waiting thread that gets to run
        ioReady = true;
        // Send a wakeup signal to one of the threads waiting using the condition variable (cv)
        cv.notify_one();
    }

    for (int i = 1; i <= NUMITEMS; i++)
    {
        // Obtain a lock based on the mtx variable.
        unique_lock<std::mutex> lck(mtx);

        // wait for the condition variable to send a wakeup signal to continue
        // Secondary check: removeFromQueue must be true
        cv.wait(lck, []{return removeFromQueue;});

        // Output mesage and remove from queue at the same time
        cout << "Consumer rem'd " << aQueue->remove() << " (qsz: " << aQueue->size() << ")" << endl;

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
    }

    {
        unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, []{return ioReady;});
        // Set the ioReady to false. Prevents competing i/o in case of spurrious wakeup
        ioReady = false;
        cout << "Consumer ending" << endl;
        // Set the ioReady variable to true to set it up for the next waiting thread that gets to run
        ioReady = true;
        // Send a wakeup signal to one of the threads waiting using the condition variable (cv)
        cv.notify_one();
    }
}