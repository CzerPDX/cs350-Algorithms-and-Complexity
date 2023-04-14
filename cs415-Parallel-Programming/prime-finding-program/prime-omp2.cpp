//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// Brooke Czerwinski

// A prime-finding program (OMP v2 version).
//
// Usage: 
//   linux> ./prime-omp2 N
//

#include <iostream>
#include <cmath> 
#include <iomanip>
#include <chrono>
#include <omp.h>


using namespace std; 

int main(int argc, char **argv) {
  // 1. [Params] (Master thread) receives values of N (and possibly P) from command-line arguments
  int N;
  int P = 1;
  if (argc < 2) {
    cout << "Usage: ./prime-omp2 N\n"; 
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

  cout << "prime (OMP v2) over [2.." << N << "] starting ...\n";

  // Mark start time
  auto start = chrono::steady_clock::now();

  //[Init] (Master thread) allocates arrays candidate[N] and sieve[√N]; it also initializes the whole candidate[] array.
  int   sqrtN = sqrt(N);
  bool  candidate[N + 1];
  int   sieve[sqrtN];

  for (int i = 2; i <= N; i++)
    candidate[i] = true;
  for (int i = 0; i < sqrtN; i++)
    sieve[i] = 0;
  

  int   sieveIdx = 0;           // Number of sieve primves added to sieve array
  bool  sieveFinished = false;  // Whether or not the sieve primes have all been found

  // Mark sieve primes
  #pragma omp parallel for shared(sieveIdx, candidate) ordered
  for (int i = 2; i <= sqrtN; i++)
  {
    #pragma omp ordered
    if (candidate[i])
    {
      // Save the prime in sieve
      //#pragma omp atomic read
      #pragma omp critical
      {
        sieve[sieveIdx] = i;
        ++sieveIdx;
      }

      // mark off multiples in the range of the sieve
      int multiple = i + i;
    
      // Mark off all of the multiples
      
      while (multiple <= sqrtN)
      {
        #pragma omp critical 
        if (candidate[multiple])
        {
          candidate[multiple] = false;
        }
        multiple += i;
      }
    }
  }
  sieveFinished = true;

  /*
    [Worker] 
    All workers compete to get the next sieve from sieve[], with a shared global index; 
    the winning worker updates the index and goes to mark off the sieve prime’s multiples in the 
    whole range (√N..N] of the candidate[] array; 
    the workers terminate when there is no more sieves to work on.

    Could emulate the competition of threads using a pool of available threads.
  */
  int workerIdx = 0;

  // Workers will mark off multiples in parallel
  #pragma omp parallel for shared(candidate, workerIdx) num_threads(P)
  for (workerIdx = 0; (workerIdx < sieveIdx); workerIdx++)
  {
    int currPrime = 0;
    int multiple = 0;
    
    #pragma omp critical
    {
      currPrime = sieve[workerIdx];
    }
    
    for (int multiple = currPrime + currPrime; multiple < (N + 1); multiple += currPrime)
    {
      #pragma omp critical
      if ((multiple > sqrtN) && (candidate[multiple] == true))
      {
        candidate[multiple] = false;
      }
      #ifdef DEBUG
      cout << "WORKER[" + to_string(omp_get_thread_num()) + "] marked " + to_string(multiple) + " as mult\n";
      #endif
    }
  }

  // Use reduction for totalPrimes
  int totalPrimes = 0;
  #pragma omp parallel shared(candidate) num_threads(P)
  {
    #pragma omp for reduction(+:totalPrimes)
    for (int i = 2; i <= N; i++)
    {
      if (candidate[i]) 
      {
        totalPrimes++;
      }
    }
  }

  // Mark completion time
  auto end = chrono::steady_clock::now();

  // Calculate duration (subtract start time from current time)
  auto duration = chrono::duration<double, std::milli> (end - start);


  cout << "prime (OMP v2) found " << totalPrimes << " primes in " << setprecision(2) << duration.count() << " ms" << endl;
}
