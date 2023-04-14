/*
    Brooke Czerwinski
    CS 415 Paralell Programming
    ProdCons1.java
    HW 1 

    Java-based producer/consumer program for parallel threads using a queue
*/

import java.util.LinkedList;
import java.util.Queue;

class ProdCons1{  
    // Required global parameters
    static int      BUFSIZE = 20;    // queue capacity
    static int      NUMITEMS = 100;  // total number of data items

    // Other global parameters
    static Object   queueAccess;                // Variable used for signalling/waiting
    static boolean  producerFinished = false;   // Whether or not the producer has finished adding to queue
    static boolean  queueFull = false;          // Whether or not the queue is full
    static boolean  queueEmpty = true;          // Whether or not the queue is empty
    static Queue<Integer> queue;                // Queue to be used throughout the program

    static Runnable producer = new Runnable()
    {
        public void run() {
            System.out.println("Producer starting");

            for (int i = 1; i <= NUMITEMS; i++)
            {
                synchronized(queueAccess)
                {
                    while (queueFull == true)
                    {
                        try{ queueAccess.wait(); }
                        catch(Exception e) { System.err.println("queueAccess.wait() = " + e.getMessage()); }
                    }

                    queue.add(i);
                    System.out.println("Producer added " + i + " (qsz: " + queue.size() + ")");

                    queueEmpty = false;
                    if (queue.size() >= BUFSIZE)
                    {
                        queueFull = true;
                    }

                    try { queueAccess.notify(); }
                    catch(Exception e) { System.err.println("queueAccess.notify() = " + e.getMessage()); }
                }
            }

            // Set producerFinished to true
            // then, in case there is a sleeping consumer thread 
            synchronized(queueAccess)
            {
                producerFinished = true;
                try { queueAccess.notify(); }
                catch(Exception e) { System.err.println("queueAccess.notify() = " + e.getMessage()); }
            }
            
            System.out.println("Producer ending");
        }
    };

    static Runnable consumer = new Runnable()
    {
        public void run() {
            boolean done = false;   // Variable to track whether or not we need to keep consuming

            System.out.println("Consumer starting");
            
            // Will keep the while loop going until there is no more to consume from the queue
            while (done != true)
            {
                // Establish that the following section requires atomic access
                synchronized(queueAccess)
                {
                    // We want to stop trying only when the producer is finished and the queue is empty
                    /*
                        ((producerFinished == true) && (queueEmpty == true)) 
                        negated = ((producerFinished != true) || (queueEmpty != true))
                    */
                    if ((producerFinished != true) || (queueEmpty != true))
                    {
                        // If the queue is empty we want to wait for a wakeup signal
                        /*
                            We will be woken up on that wait variable, but we need to make sure it's really for us.
                                For example, it could be another consumer thread signalling the producer that it has removed something
                                rather than the producer notifying us. So check for suprrious wakeup using while loop.
                            If it's not our turn we'll give up the lock so another blocked thread can use the signal
                        */
                        while (queueEmpty == true)
                        {  
                            // Wait for wakeup signal to reaquire access to the section
                            try { queueAccess.wait(); }
                            catch(Exception e) { System.err.println("queueAccess.wait() = " + e.getMessage()); }
                        }

                        // Remove item from the queue and print out to the user
                        System.out.println("Consumer rem'd " + queue.remove() + " (qsz: " + queue.size() + ")");
                        // Set queueFull to false since we know we have now taken at least one object out of it
                        queueFull = false;
                        // Check to see if that was the last item in the queue. Set queueEmpty to true, if so.
                        if (queue.size() == 0)
                        {
                            queueEmpty = true;
                        }

                        // Notify a thread that it can unlock
                        try { queueAccess.notify(); }
                        catch(Exception e) { System.err.println("queueAccess.notify() = " + e.getMessage()); }
                    }
                    else 
                    {
                        done = true; 
                    }
                }
            }
            
            System.out.println("Consumer ending");
        }
    };



    public static void main(String args[])  
    {
        // Initialize a queue
        queue = new LinkedList<>();
        queueAccess = new Object();

        // Initialize two threads to run producer() and consumer()
        Thread producerThread = new Thread(producer);
        Thread consumerThread = new Thread(consumer);
        
        try{
            // Start the two threads
            consumerThread.start();     // start consumer
            producerThread.start();     // start producer

            // Wait for the two threads to join back
            producerThread.join();      // Wait for producer to finish
            consumerThread.join();      // Wait for consumer to finish
        } catch (Exception e) { System.err.println(e.getMessage()); }

        System.out.println("Main: all done!");
    }
}