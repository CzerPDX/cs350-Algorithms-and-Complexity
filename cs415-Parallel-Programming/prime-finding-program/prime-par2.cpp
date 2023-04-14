//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// Brooke Czerwinski

// A prime-finding program (C++ proper version).
//
// Usage: 
//   linux> ./prime-par2 N
//

#include <atomic>
#include <chrono>
#include <cmath> 
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std; 


atomic<int>         totalPrimes (0);        // Total # of primes found in run
int                 N;											// primes will be found from 0 to N
int                 sqrtN;									// floor(sqrt(N))
bool                *candidate;							// array containing candidate primes 0 - N
int                 *sieve;									// array containing sieve primes found
mutex               mtx;										// mutex variable used with condition variable cv
condition_variable  cv;											// condition variable to notify worker threads
bool                sieveFinished = false;	// Whether or not the sieve has finished.
int                 sieveIdx = 0;           // Number of items that have been added to sieveArray


// Worker thread runs concurrently over a partition of the work
void worker(const int id, const int startIdx, const int endIdx);


int main(int argc, char **argv) {
  int P = 1;
  if (argc < 2) {
    cout << "Usage: ./prime-par2 N\n"; 
    exit(0);
  }
  if ((N = atoi(argv[1])) < 2) {
    cout << "N must be greater than 1\n"; 
    exit(0);
  }
  // optional 2nd command line argument
  if (argc > 2)
  {
    if ((P = atoi(argv[2])) <= 0)
    {
      cout << "P must be greater than 0\n";
      exit(0);
    }
  }
  cout << "prime (C++ v2) starting ...\n";

  sqrtN = sqrt(N);      // Square root of N (integer)

  // Mark start time
  auto  start = chrono::steady_clock::now();        // Start time
  
  // [Init] (Master thread) Allocate arrays candidate[N] and sieve[√N].
  // candidate = new bool[N+1];
  candidate = new bool[N + 1];
  sieve = new int[sqrtN];

  // [Start-Threads] (Master thread) Create P threads (worker[0]..worker[P-1])
  thread threadArr[P];

  int partitionSize = (N - sqrtN) / P;  // Size of each parittion
  int offset = (N - sqrtN) % P;         // Remainder or offset for uneven divisions

  #ifdef DEBUG
  cout << endl;
  cout << "==================================================================" << endl;
  cout << "DEBUG INFO (How work will be partitioned):" << endl;
  cout << "sqrtN = " << sqrtN << endl;
  cout << endl;
  cout << "[0,1] are always basically ignored\n";
  cout << "[2," << sqrtN << "] of candidate will be marked by SIEVE" << endl;
  cout << "[" << (sqrtN + 1) << "," << N << "] of candidate will be marked by WORKERS" << endl;
  cout << endl;
  cout << "Workers will handle the final " << (N - sqrtN) << " indexes of the candidate array." << endl;
  cout << "Base partition size: " << partitionSize << endl;
  cout << "Remainder/offset:    " << offset << endl;
  if (offset > 0)
  {
    cout << "Due to a nonzero offset, worker(s) 0 through " << (offset - 1) << " will process an additional index of the candidate array" << endl;
  }
  cout << endl;
  #endif

  // Initialize threads
  for (int id = 0; id < P; id++)
  {
    threadArr[id] = thread(worker, id, partitionSize, offset);
  }


  // [Sieve] (Master thread) (a) Initialize (only) the sieve section [2..√N] of candidate[]; 
  for (int i = 2; i <= sqrtN; i++)
  {
    candidate[i] = true;
  }
  
  sieveIdx = 0;
  // [Sieve] (Master thread)
  // (b) Finds all sieve primes in the sieve section and save them in sieve[];
  // (c) Adds the total number of sieves to the global variable totalPrimes
  for (int i = 2; i <= sqrtN; i++)
  {
    // If a candidate number is marked as true, then it is a prime
    if (candidate[i])
    {
      {
        // Get the lock
        unique_lock<std::mutex> lck(mtx);
        sieve[sieveIdx] = i;          // Save the prime to sieve index

        sieveIdx++;                   // Increment next index that a prime should be saved to
        totalPrimes++;                // Add one to the prime count

        // Mark each multiple of the prime i as false up to sqrtN
        for (int j = i+i; j <= sqrtN; j += i)
        {
          candidate[j] = false;
        }
        
        cv.notify_all();              // Send out a notification for any threads waiting for a new sieveIdx
      }
    }
  }
  {
    unique_lock<std::mutex> lck(mtx);
    sieveFinished = true;

    cv.notify_all();
  }

  

  // [End-Threads] Master waits for workers to join back, and prints out the final totalPrimes result
  // Join threads back together
  for (int i = 0; i < P; i++)
  {
    threadArr[i].join();
  }
 

  // Mark completion time
  auto end = chrono::steady_clock::now();

  // Calculate duration (subtract start time from current time)
  auto duration = chrono::duration<double, std::milli> (end - start);

  #ifdef DEBUG
  cout << "==================================================================" << endl;
  cout << endl;
  #endif

  // Print out final total primes result
  cout << "prime (C++ proper) found " << totalPrimes << " primes in " << setprecision(2) << duration.count() << " ms" << endl;
}



/*
  [Worker] Each worker 
  (a) figures out its corresponding section in the range (√N..N], and initializes that section of candidate[]; 
  (b) it walks through the sieve primes in sieve[], and marks off their multiples in its section of candidate[]; 
  (c) it tallies the primes found in its section, and adds the number to totalPrimes.
*/
void worker(const int id, const int partitionSize, const int offset)
{
  // (a.1) Figure out what section the thread is responsible for
  int startIdx = partitionSize * id + sqrtN;      // Start of index work range (exclusive)
  int endIdx = partitionSize * (id + 1) + sqrtN;  // End of index work range (inclusive)

  // (a.2) Adjust the start and ending Idx if remainder/offset is not 0
  if (offset > 0)
  {
    // Some threads will have one additional spot they are responsible for as illustrated above.
    if (id < offset)
    {
      startIdx += id;
      endIdx += id + 1;
    }
    // Other index ranges need to shift over to accommodate the additional ones distributed previous to it.
    else
    {
      startIdx += offset;
      endIdx += offset;
    }
  }

  #ifdef DEBUG
  // Worker range is figured out now.
  cout << "Worker[" + to_string(id) + "] on range (" + to_string(startIdx) + "..." + to_string(endIdx) + "]\n";
  #endif
  
  /*
    Note to self:
    The project requirements state that the workers need to iterate over the range (√N..N]
    This means that each worker should be responsible for a set that is (startIdx..endIdx]

    The shift from [startIdx,endIdx) to (startIdx,endIdx] requires a 1-index instead of a 
    0-index for the for loops. Make the following changes to the base/standard for loop form:

          // startIdx = exclusive (so add + 1 whenever we use i startIdx)
          // endIdx = inclusive (so use <= whenever we compare i to endIdx)

    0-indexed: for (int i = startIdx; i < endIdx; i++) {}         //  [startIdx, endIdx)
    1-indexed: for (int i = startIdx + 1; i <= endIdx; i++) {}    //  (startIdx, endIdx]
  */
  
  // (a.3) Initialize section of candidate that this worker thread is responsible for
  for (int i = startIdx + 1; i <= endIdx; i++)
  {
    candidate[i] = true;
  }

  /*
    Note to self: 
    sieveIdx represents the number of items that have been added to the sieve array.
    This means that it will only ever be safe for us to check the index that is 1 less than
    the current value of sieveIdx. Or rather, when 
          workerSieveIdx < sieveIdx
    
    Loop and wait conditions here were hard to figure out.
    Notes below on how I figured them out so I can follow for future reference

    sieveFinished | i < sieveIdx  | stop loop | wait for notify
          T       |       T       |     F     |       F           
          T       |       F       |     T     |       F
          F       |       T       |     F     |       F
          F       |       F       |     F     |       T

    The above truth table matches the following one, so they are equivalent:

    P | Q | ~(P -> Q) | (~P AND ~Q)
    T | T |     F     |     F
    T | F |     T     |     F
    F | T |     F     |     F
    F | F |     F     |     T
    
    Where:
    P:            (sieveFinished == true)
    Q:            (i < sieveIdx)
    ~(P -> Q):    stop the outer loop
    (~P AND ~Q):  wait until a notification is received

    "When to keep looping" is more useful than "when to stop looping", so negate it and simplify:
    ~(~(P -> Q)) == (P -> Q) == (~P OR Q)

    Therefore:
    The outer loop will be a while loop with a condition of (~P OR Q):  
          while ((sieveFinished == false) || (i < sieveIdx))
    The inner wait condition will be (~P ^ ~Q):
          if ((sieveFinished != true) && (i >= sieveIdx))
  */
  int workerSieveIdx = 0;     // Keeps track of this worker's next sieveIdx to use

  while (sieveFinished != true || (workerSieveIdx < sieveIdx))
  {
    {
      unique_lock<std::mutex> lck(mtx);
      // If this worker thread has outpaced the sieve it should wait until it is notified that more sieve primes
      // have been added to the sieve array through the condition variable (cv)
      if (workerSieveIdx >= sieveIdx)
			{
				// After waking, double check that the workerIdx is actually less than the sieveIdx.
				cv.wait(lck, [workerSieveIdx]{return ((workerSieveIdx < sieveIdx) || sieveFinished == true);});
			}
      // Once we know there is something to add from sieve we can actually release the lock
    }

    if ((sieveFinished != true) || (workerSieveIdx < sieveIdx))
    {
      // Take care of all sieve multiples from (startIdx,endIdx]
      int multiples = sieve[workerSieveIdx] + sieve[workerSieveIdx];
      // Get into range for this worker's section
      while (multiples <= endIdx)
      {
        // If any of the multiples fall between (startIdx,endIdx] in candidate, set that index to false.
        if (multiples > startIdx)
        {
          candidate[multiples] = false;
        }
        multiples += sieve[workerSieveIdx];
      }
      workerSieveIdx++;
    }
  }

  // (c) Tally the primes found in its section, and add the number to totalPrimes
  int tally = 0;
  for (int i = startIdx + 1; i <= endIdx; i++)
  {
    if (candidate[i] == true)
    {
      tally++;
    }
  }
  totalPrimes += tally;
}