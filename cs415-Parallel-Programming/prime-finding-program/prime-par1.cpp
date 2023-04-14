//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// Brooke Czerwinski

// A prime-finding program (C++ naive version).
//
// Usage: 
//   linux> ./prime-par1 N
//

#include <atomic>
#include <chrono>
#include <cmath> 
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std; 

atomic<int> totalPrimes (0);        // Total # of primes found in run
int   N;
int   sqrtN;
bool  *candidate;
int   *sieve;



void worker(const int id, const int startIdx, const int endIdx);


int main(int argc, char **argv) {
  int P = 1;
  if (argc < 2) {
    cout << "Usage: ./prime-par1 N\n"; 
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

  
  cout << "prime (C++ v1) starting ...\n";

  sqrtN = sqrt(N);      // Suare root of N (integer)

  // Mark start time
  auto  start = chrono::steady_clock::now();        // Start time
  
  // [Init] (Master thread) Allocate arrays candidate[N] and sieve[√N].
  candidate = new bool[N+1];
  sieve = new int[sqrtN];

  for (int i = 0; i < sqrtN; i++)
  {
    sieve[i] = -1;
  }

  // [Sieve] (Master thread) (a) Initialize (only) the sieve section [2..√N] of candidate[]; 
  for (int i = 2; i <= sqrtN; i++)
  {
    candidate[i] = true;
  }
  
  int sieveIdx = 0;
  // [Sieve] (Master thread)
  // (b) Finds all sieve primes in the sieve section and save them in sieve[];
  // (c) Adds the total number of sieves to the global variable totalPrimes
  for (int i = 2; i <= sqrtN; i++)
  {
    // If a candidate number is marked as true, then it is a prime
    if (candidate[i])
    {
      sieve[sieveIdx] = i;          // Save the prime to sieve index
      sieveIdx++;                   // Increment next index that a prime should be saved to
      totalPrimes++;                // Add one to the prime count

      // Mark each multiple of the prime i as false up to sqrtN
      for (int j = i+i; j <= sqrtN; j += i)
      {
        candidate[j] = false;
      }
    }
  }

  // [Start-Threads] (Master thread) Create P threads (worker[0]..worker[P-1])
  thread threadArr[P];

  int partitionSize = (N - sqrtN) / P;  // Size of each parittion
  int offset = (N - sqrtN) % P;         // Remainder or offset for uneven divisions

  // Initialize threads
  for (int id = 0; id < P; id++)
  {
    threadArr[id] = thread(worker, id, partitionSize, offset);
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

  // Print out final total primes result
  cout << "prime (C++ v1) found " << totalPrimes << " primes in " << setprecision(2) << duration.count() << " ms" << endl;
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
  int startIdx = partitionSize * id + sqrtN;      // Start of index work range (inclusive)
  int endIdx = partitionSize * (id + 1) + sqrtN;  // End of index work range (exclusive)

  // (a.2) Adjust the start and ending Idx if remainder is not 0
  if (offset > 0)
  {
    /*
      The following handles uneven partitions (ie, when (N - sqrtN) mod P doesn't equal 0)

      If (N - sqrtN) = 18 and P = 6, remainder (offset) is 0
      | 0 | 0 | 0 | 1 | 1 | 1 | 2 | 2 | 2 | 3 | 3 | 3 | 4 | 4 | 4 | 5 | 5 | 5 |

      If (N - sqrtN) = 19 and P = 6, remainder (offset) is 1
      | 0 | 0 | 0 | 0 | 1 | 1 | 1 | 2 | 2 | 2 | 3 | 3 | 3 | 4 | 4 | 4 | 5 | 5 | 5 |
                    ^
                    |
                  extra

      If (N - sqrtN) = 21 and P = 6, remainder (offset) is 3
      | 0 | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 2 | 2 | 2 | 2 | 3 | 3 | 3 | 4 | 4 | 4 | 5 | 5 | 5 |
                    ^               ^               ^
                    |               |               |
                  extra           extra           extra

      If the id of the thread is less than the offset, then add one additional one to its partitionSize
      Everything else must shift over to accommodate these larger sections
    */

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
  cout << "Worker[" + to_string(id) + "] on range (" + to_string(startIdx) + "..." + to_string(endIdx) + "]\n";
  #endif
  

  // (a.3) Initialize section of candidate that this worker thread is responsible for
  for (int i = startIdx + 1; i <= endIdx; i++)
  {
    candidate[i] = true;
  }

  // (b) Walk through the sieve primes in sieve[], and mark off their multiples in its section of candidate[]
  for (int i = 0; ((i < sqrtN) && (sieve[i] != -1)); i++)
  {
    // Add up enough times to get into range we are responsible for
    for (int j = sieve[i] + sieve[i]; j <= endIdx; j += sieve[i])
    {
      if ((j > startIdx) && (candidate[j] != false))
      {
        // Then mark the multiples of that prime within our section
        candidate[j] = false;
      }
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