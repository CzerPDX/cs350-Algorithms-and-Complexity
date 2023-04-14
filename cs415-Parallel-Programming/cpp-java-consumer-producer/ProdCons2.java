/*
    Brooke Czerwinski
    CS 415 Paralell Programming
    ProdCons2.java
    HW 1 

    Java-based producer/consumer program for parallel threads using a queue
    
    The program takes an optional command-line argument, numCons (number of consumers). If it is not
    provided, a default value of 1 is used.


    References used:
    https://stackoverflow.com/questions/5853167/runnable-with-a-parameter
    https://www.w3schools.com/java/java_lambda.asp
*/

import java.util.LinkedList;
import java.util.Queue;

public class ProdCons2{  

    // Required global parameters
    static int      BUFSIZE = 5;    // queue capacity
    static int      NUMITEMS = 100;  // total number of data items

    // Other global parameters
    static String   queueAccess = "queueAccess";    // Variable used for signalling/waiting
    static boolean  producerFinished = false;       // Whether or not the producer has finished adding to queue
    static boolean  queueFull = false;              // Whether or not the queue is full
    static boolean  queueEmpty = true;              // Whether or not the queue is empty
    static Queue<Integer> queue;                    // Queue to be used throughout the program
    static ConsumerClass[]  consumers;              // Holds the array of consumer threads
    static int numCons = 1;                         // Number of consumer threads. Can be changed with argument. Default is 1

    

    // ProducerClass which extends from thread
    // helps in the creation of producer threads.
    static class ProducerClass extends Thread
    {
        // Constructor
        ProducerClass() { super(); }

        // Override the run method
        @Override
        public void run() 
        {
            int count = 1;  // Number of items we have put into the queue (2-indexed)
            int num = 0;    // 

            System.out.println("Producer starting");

            // We are adding more than just NUMITEMS to the queue
            // After NUMITEMS have been added, we need to add numCons worth of -1's to the queue
            while (count <= (NUMITEMS + numCons))
            {
                synchronized(queueAccess)
                {
                    // If the queue is full the method waits on queueAccess to wake it
                    // Releases lock for now and release lock for now
                    while (queueFull == true)
                    {
                        try { queueAccess.wait(); }
                        catch (Exception e) { System.err.println(e.getMessage()); }
                    }

                    // Once the queue has room to add to it, we need to figure out what we're adding
                    // If we still have regular numbers to add we add our current count
                    if (count <= NUMITEMS)
                    {
                        num = count;
                    }
                    // Otherwise we add as many -1's to the queue as there are consumer threads
                    else
                    { 
                        num = -1; 
                    }

                    // Add the number to the qeueue, then print it out with the queue size.
                    queue.add(num);
                    System.out.println("Producer added " + num + " (qsz: " + queue.size() + ")");

                    // There is no way the queue is empty because we added something so set it to queueEmpty to false
                    queueEmpty = false;
                    // If the queue is now full after we've added something, set it to queueFull to true.
                    if (queue.size() >= BUFSIZE)
                    {
                        queueFull = true;
                    }

                    // Notify all threads to check to see if they can run again.
                    try { queueAccess.notifyAll(); }
                    catch (Exception e) { System.err.println(e.getMessage()); }
                }
                count++;
            }

            System.out.println("Producer ending");
        }
    }



    static class ConsumerClass extends Thread
    {
        int k;  // consumer thread id # given by main.

        ConsumerClass(int incoming) { 
            super(); 
            k = incoming;
        }

        @Override
        public void run()
        {
            int     last = 0;       // Last integer to be removed from the queue
            String  removed = "";   // String that will be displayed based on what was removed

            System.out.println("Consumer[" + k + "] running");

            // Keep removing things from the queue until a -1 has been removed.
            while (last != -1)
            {
                synchronized(queueAccess)
                {
                    while (queueEmpty == true)
                    {
                        try { queueAccess.wait(); }
                        catch (Exception e) { System.err.println(e.getMessage()); }
                    }

                    // Remove item from the queue and print out to the user
                    last = queue.remove();
                    if (last != -1)
                    {
                        removed = String.valueOf(last);
                    }
                    else
                    {
                        removed = "an ENDSIGN";
                    }
                    System.out.println("Consumer[" + k + "] rem'd " + removed + " (qsz: " + queue.size() + ")");

                    // Queue cannot be full because we just removed something from it, so set queueFull to false
                    queueFull = false;
                    // If the queue is now empty, set queueEmpty to true
                    if (queue.size() <= 0)
                    {
                        queueEmpty = true;
                    }

                    // Notify all threads waiting on queueAccess that they can try to get the lock again.
                    try { queueAccess.notifyAll(); }
                    catch (Exception e) { System.err.println(e.getMessage()); }
                }
            }

            System.out.println("Consumer[" + k + "] ending");
        }
    }

    public static void main(String args[])  
    {
        // If a parameter was provided, set numCons to that value
        if (args.length > 0)
        {
            numCons = Integer.parseInt(args[0]);  
        }

        // Initialize a queue
        queue = new LinkedList<>();
        
        // Create producer and consumer threads
        ProducerClass producer = new ProducerClass(); 
        consumers = new ConsumerClass[numCons]; 
        // Fill consumer thread array with ConsumerClass threads
        for (int i = 0; i < numCons; i++)
        {
            consumers[i] = new ConsumerClass(i);
        }

        // Start all producer and consumer threads
        producer.start();
        for (int i = 0; i < numCons; i++)
        {
            consumers[i].start();
        }

        // Join all producer and consumer threads back together
        try { producer.join(); }
        catch (Exception e) { System.err.println(e.getMessage()); }
        for (int i = 0; i < numCons; i++)
        {
            try { consumers[i].join(); }
            catch (Exception e) { System.err.println(e.getMessage()); }
        }    

        System.out.println("Main: all done!");
    }
}