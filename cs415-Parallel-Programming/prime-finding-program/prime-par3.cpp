//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// Brooke Czerwinski

// A prime-finding program (C++ Third multithreaded version).
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

int                 N;													// primes will be found from 0 to N
int                 sqrtN;											// floor(sqrt(N))
bool                *candidate;									// array containing candidate primes 0 - N
int                 *sieve;											// array containing sieve primes found
mutex               mtx;												// mutex variable used with condition variable cv
condition_variable  cv;													// condition variable to notify worker threads
bool                sieveFinished = false;			// Whether or not the sieve has finished.
atomic<int>					workerIdx (0);							// Global shared index for worker threads
int                 sieveIdx = 0;           		// Number of items that have been added to sieveArray
int									*stats;

void worker(const int id);

int main(int argc, char **argv) {
	// 1. [Params] Master receives values of N (and possibly P) from command-line arguments.
  int P = 1;
  if (argc < 2) {
    cout << "Usage: ./prime-par3 N\n"; 
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
	cout << "prime (C++ v3) starting ...\n";

  // Mark start time
  auto  start = chrono::steady_clock::now();

	sqrtN = sqrt(N);      							// Square root of N (integer)
  
  // 2.1 [Init] (Master thread) Allocate arrays candidate[N] and sieve[√N].
  candidate = new bool[N + 1];
  sieve = new int[sqrtN];

	// Allocate stats array and initialize to 0
	stats = new int[P];
	for (int i = 0; i < P; i++)
	{
		stats[i] = 0;
	}

  // 2.2 [Init] (Master thread) Initialize the whole candidate[] array
	candidate[0], candidate[1] = false;
  for (int i = 2; i < (N + 1); i ++)
  {
		candidate[i] = true;
  }

  // 3. [Start-Threads] (Master thread) Create P threads (worker[0]..worker[P-1])
  thread threadArr[P];								// An array to hold P threads
  // Initialize threads in threadArray
  for (int id = 0; id < P; id++)
  {
    threadArr[id] = thread(worker, id);
  }

  // 4. [Sieve] (Master thread) Master works over the range [2..√N] and finds all sieves, 
	//    and save them in the sieve[] array; after each sieve is found, it notifies the workers.	
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
        
        cv.notify_one();              // Send out a notification for any threads waiting for a new sieveIdx
      }
    }
  }
  {
    unique_lock<std::mutex> lck(mtx);
    sieveFinished = true;

		// Wake em all up so they can shut down
    cv.notify_all();
  }

  

  // [End-Threads] Master waits for workers to join back, and prints out the final totalPrimes result
  // Join threads back together
  for (int i = 0; i < P; i++)
  {
    threadArr[i].join();
  }

	// Count primes
	int totalPrimes = 0;
	for (int i = 0; i < (N + 1); i++)
	{
		if (candidate[i])
		{
			totalPrimes++;
		}
	}

  // Mark completion time
  auto end = chrono::steady_clock::now();

  // Calculate duration (subtract start time from current time)
  auto duration = chrono::duration<double, std::milli> (end - start);
	

  // Print out final total primes result
  cout << "prime (C++ v3) found " << totalPrimes << " primes in " << setprecision(2) << duration.count() << " ms" << endl;

	#ifdef DEBUG
	cout << "Number of primes completed by each worker thread:" << endl;
	for (int i = 0; i < P; i++)
	{
		cout << "Worker[" << i << "]: " << stats[i] << endl;
	}
	#endif
}



/*
  [Worker] 
	All workers compete to get the next sieve from sieve[], with a shared global index; the 
	winning worker updates the index and goes to mark off the sieve prime’s multiples 
	in the whole range(√N..N] of the candidate[] array; 
	the workers terminate when there is no more sieves to work on.
*/
void worker(const int id)
{
	int currentPrime = 0;
	while ((sieveFinished != true) || (workerIdx < sieveIdx))
	{
		{
			// Try to get the lock
			unique_lock<std::mutex> lck(mtx);
			// If the global workerIdx < sieveIdx, then the worker can proceed
			// Otherwise, it must wait to be woken by a notify
			if (workerIdx >= sieveIdx)
			{
				// After waking, double check that the workerIdx is actually less than the sieveIdx.
				cv.wait(lck, []{return ((workerIdx < sieveIdx) || sieveFinished == true);});
			}
			// Skip ahead when sieveFinished == true and workerIdx >= sieveIdx
			// negated would be (sieveFinished == false OR workerIdx < sieveIdx)
			// Otherwise, grab the prime
			if ((sieveFinished != true) || (workerIdx < sieveIdx))
			{
				// save the current prime
				currentPrime = sieve[workerIdx];
				// update the shared global index
				workerIdx++;
			}
			// Release the lock
		}
		
		// Now do the work and remove the multiples (unless currentPrime <= 0)
		if (currentPrime > 0)
		{
			#ifdef DEBUG
			cout << "Worker[" + to_string(id) + "] removing multiples of " + to_string(currentPrime) + "...\n";
			#endif

			// Set all multiples to false
			for (int multiple = currentPrime + currentPrime; multiple < (N + 1); multiple += currentPrime)
			{
				// Possible race condition here, but because any conflicting threads would only be writing false, it should be fine.
				candidate[multiple] = false;
			}

			// increment stats[id] once per prime to keep track of how many primes this thread has worked on
			stats[id]++;

			// Reset current prime for next round
			currentPrime = 0;
		}
	}
	
}