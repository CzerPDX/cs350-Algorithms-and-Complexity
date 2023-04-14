//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// Brooke Czerwinski

// A prime-finding program (Open MP version).
//
// Usage: 
//   linux> ./prime-omp N
//
#include <iostream>
#include <cmath> 
#include <iomanip>
#include <chrono>
#include <omp.h>


using namespace std; 

int main(int argc, char **argv) {
  int N;
  int P = 1;
  if (argc < 2) {
    cout << "Usage: ./prime-omp N\n"; 
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

  cout << "prime (OMP v1) over [2.." << N << "] starting ...\n";

  // Mark start time
  auto start = chrono::steady_clock::now();

  // Loop #1
  bool candidate[N+1];          // An array to keep track of primes (true = prime)

  #pragma omp parallel for shared(candidate) num_threads(P)
  // Fill the array with "true" from index 2 til the end
  for (int i = 2; i <= N; i++) 
  {
    candidate[i] = true;
  }
    
  // Loop #2 (loopnest)
  /*
    The outer loop starts a new set of marking things off.
    We don't want it to do extra work, so the outer portion
    should not be parallel
  */
  int sqrtN = sqrt(N);
  for (int i = 2; i <= sqrt(N); i++)
  {
    if (candidate[i]) 
    {
      #pragma omp parallel for shared(candidate) num_threads(P)
      for (int j = i+i; j <= N; j += i)
      {
        candidate[j] = false;
      }
    }
  }
    
  // Loop #3
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

  cout << "prime-omp (" << P << " threads) found " << totalPrimes << " primes in " << setprecision(2) << duration.count() << " ms" << endl;

  #ifdef SAVEFILE
  cout << setprecision(2) <<  << setprecision(2) << duration.count() << " ms" << endl;
  #endif

}
