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

/*
    static class ConsumerClass extends Thread {
        String incoming;
        ConsumerClass(int n) 
        { 
            super(Integer.toString(n)); 
            incoming = Integer.toString(n);
        }

        @Override
        public void run() 
        {
            int k = Integer.parseInt(incoming);
            boolean done = false;   // Variable to track whether or not we need to keep consuming
            int last = 0;

            System.out.println("Consumer[" + k + "] starting");
            
            // Will keep the while loop going until there is no more to consume from the queue
            while (last != -1)
            {
                // Establish that the following section requires atomic access
                synchronized(queueAccess)
                {
                    System.out.println("Entering synchronized queueAccess -------------  Consumer[" + k + "]");
                    // Wait until the queue isn't empty
                    while (queueEmpty == true)
                    {  
                        // Wait for wakeup signal to reaquire access to the section
                        try{ 
                            System.out.println("------  Consumer[" + k + "] ------- +++++++++++ SLEEP");
                            queueAccess.wait(); 
                            System.out.println("------  Consumer[" + k + "] ------- WOKE UP");
                        }
                        catch(Exception e) { System.err.println("queueAccess.wait() = " + e.getMessage()); }
                    }

                    // Remove item from the queue and print out to the user
                    last = queue.remove();
                    System.out.println("Consumer[" + k + "] rem'd " + last + " (qsz: " + queue.size() + ")");
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
            }
            
            System.out.println("Consumer[" + k + "] ending");
        }
    }
*/


    /*
    //static void producer(int numCons)
    {
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


        for (int i = 0; i < numCons; i++)
        {
            synchronized(queueAccess)
            {
                while (queueFull == true)
                {
                    try{ 
                        queueAccess.wait();
                    }
                    catch(Exception e) { System.err.println("queueAccess.wait() = " + e.getMessage()); }
                }

                System.out.println("Producer adding -1 (qsz: " + queue.size() + ")");
                queue.add(-1);
                queueEmpty = false;
                if (queue.size() >= BUFSIZE)
                {
                    queueFull = true;
                }

                try { queueAccess.notify(); }
                catch(Exception e) { System.err.println("queueAccess.notify() = " + e.getMessage()); }
            }
        }
        System.out.println("Producer ending");
    }
    */

    
    /*
    public Runnable producer = new Runnable() 
    {
        public void run() {
            String name = Thread.currentThread().getName();
            System.out.println("Producer " + name + " running");
        }
    };


    public Runnable consumer = new Runnable() 
    {
        public void run() {
            String name = Thread.currentThread().getName();
            System.out.println("Consumer " + name + " running");

            for (int i = 0; i < BUFSIZE; i++)
            {
                System.out.println("Consumer " + name + " = " + i);
            }
        }
    };

    */



/*
    

    static void producer(int numCons)
    {
        String name = Thread.currentThread().getName();
        System.out.println("Producer " + name + " running...");

        for (int i = 0; i < BUFSIZE; i++)
        {
            synchronized(pingpong)
            {
                while (ping != true)
                {
                    try { pingpong.wait(); }
                    catch (Exception e) { System.err.println(e.getMessage()); }
                }
                System.out.println("Producer " + name + " = " + i);
                ping = false;
                pong = true;
                pingpong.notify();
            }
        }
    }


    static void consumer(int k) {
        String name = Thread.currentThread().getName();
        System.out.println("Consumer[" + k + "] running");

        for (int i = 0; i < BUFSIZE; i++)
        {
            synchronized(pingpong)
            {
                while (pong != true)
                {
                    try { pingpong.wait(); }
                    catch (Exception e) { System.err.println(e.getMessage()); }
                }
                System.out.println("Consumer[" + k + "] = " + i);
                ping = true;
                pong = false;
                pingpong.notify();
            }
        }
    }
*/




    // Required global parameters
    static int      BUFSIZE = 5;    // queue capacity
    static int      NUMITEMS = 100;  // total number of data items

    // Other global parameters
    static Object   queueAccess;                // Variable used for signalling/waiting
    static boolean  producerFinished = false;   // Whether or not the producer has finished adding to queue
    static boolean  queueFull = false;          // Whether or not the queue is full
    static boolean  queueEmpty = true;          // Whether or not the queue is empty
    static Queue<Integer> queue;                // Queue to be used throughout the program    
    static ProducerClass[]  producers;
    static ConsumerClass[]  consumers;



    static String  pingpong = "pingpong";
    static boolean ping = true;
    static boolean pong = false;

    

    static class ProducerClass extends Thread
    {
        int k;

        ProducerClass(int n) {
            super(); 
            k = n;
        }

        @Override
        public void run() 
        {
            System.out.println("Producer " + k + " running...");
            int count = 0;

            while (count < BUFSIZE)
            {
                synchronized(pingpong)
                {
                    System.out.println("Producer[" + k + "] ENTERING LOCK");
                    while (ping == true)
                    {
                        System.out.println("Producer[" + k + "] ++++++++++++++++ WAITING");
                        try { pingpong.wait(); }
                        catch (Exception e) { System.err.println(e.getMessage()); }
                    }
                    //pong = false;
                    System.out.println("Producer[" + k + "] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Acquired lock PONG = TRUE");
                    System.out.println(">>>>>> (" + k + ") PONG !!!!!!!!!!!!!!!!!!!!!!");
                    //System.out.println("Producer " + name + " = " + count);
                    ping = true;
                    //pong = true;]
                    System.out.println("Producer[" + k + "] RELEASING LOCK");
                    pingpong.notifyAll();
                    count++;
                }
            }
        }
    }

    static class ConsumerClass extends Thread
    {
        int k;

        ConsumerClass(int incoming) { 
            super(); 
            k = incoming;
        }

        @Override
        public void run()
        {
            System.out.println("Consumer[" + k + "] running");
            int count = 0;

            while (count < BUFSIZE)
            {
                synchronized(pingpong)
                {
                    System.out.println("Consumer[" + k + "] ENTERING LOCK");
                    while (ping == false)
                    {
                        System.out.println("Consumer[" + k + "] ++++++++++++++++ WAITING");
                        try { pingpong.wait(); }
                        catch (Exception e) { System.err.println(e.getMessage()); }
                    }
                    ping = false;
                    System.out.println("Consumer[" + k + "] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Acquired lock PING = TRUE");
                    System.out.println(">>>>>> (" + k + ") PING ??????????????????????");
                    //System.out.println("Consumer[" + k + "] = " + count);
                    //pong = true;
                    //pong = false;
                    System.out.println("Consumer[" + k + "] RELEASING LOCK");
                    pingpong.notifyAll();
                    count++;
                }
            }
        }
    }

    static void createThreads(int numCons)
    {
        //ConsumerClass test = new ConsumerClass(numCons);
    }

    public static void main(String args[])  
    {
        int numCons = 1;

        if (args.length > 0)
        {
            numCons = Integer.parseInt(args[0]);  
        }

        System.out.println("NUMCONS = " + numCons);

        // Initialize a queue
        queue = new LinkedList<>();
        // queueAccess = new Object();

        // Initialize threads
        producers = new ProducerClass[numCons];
        consumers = new ConsumerClass[numCons];

        // Fill arrays with threads
        for (int i = 0; i < numCons; i++)
        {
            final int finalNumCons = numCons;
            final int finalI = i;
            producers[i] = new ProducerClass(i);
            consumers[i] = new ConsumerClass(i);
            //producers[i] = new Thread(() -> producer(finalNumCons));
            //consumers[i] = new Thread(() -> consumer(finalI));
        }
        // Start the threads
        for (int i = 0; i < numCons; i++)
        {
            producers[i].start();
            consumers[i].start();
        }

        // Join the threads back together
        for (int i = 0; i < numCons; i++)
        {
            try { producers[i].join(); }
            catch (Exception e) { System.err.println(e.getMessage()); }

            try { consumers[i].join(); }
            catch (Exception e) { System.err.println(e.getMessage()); }
        }    

        /*
        ConsumerClass threadList[] = new ConsumerClass[numCons];

        for (int i = 0; i < numCons; i++)
        {
            threadList[i] = new ConsumerClass(i);
        }

        // Initialize Producer
        // Use a lambda so you can input a parameter
        final int finalNumCons = numCons;
        Thread producerThread = new Thread(() -> producer(finalNumCons));

        
        try{
            // start consumer threads
            for (int i = 0; i < numCons; i++)
            {
                threadList[i].start();
            }
            // start producer thread
            producerThread.start();

            // Wait for the threads to join back
            for (int i = 0; i < numCons; i++)
            {
                threadList[i].join();
            }
            producerThread.join();      // Wait for producer to finish

        } catch (Exception e) { System.err.println(e.getMessage()); }
        */

        System.out.println("Main: all done!");
    }
}