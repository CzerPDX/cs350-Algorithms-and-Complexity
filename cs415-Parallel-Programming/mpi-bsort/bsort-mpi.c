//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// Brooke Czerwinski

// bsort-mpi.c  -  Reads input data from a file, and writes sorted result to another file.
//
// Usage: 
//    linux> mpirun -n P bsort-mpi <infile> <outfile> (P must be a power of 2) (data in infile must be a multiple of P)
//           -- use P buckets to sort data in <infile> and write result to <outfile>
//
//    mpicc -o bsort-mpi bsort-mpi.c -lm         // compile 
// 
/*
  References: https://www.mpich.org/static/docs/v3.1/www3/MPI_Iprobe.html 
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define DATABITS 13     // assume data are 13-bit integers: [0,8191] 

// return true if x is a power of 2
#define IsPowerOf2(x) (!((x) & ((x) - 1)))

// bucket index for integer x of b bits (B is #buckets)
#define BktIdx(x,b,B) ((x) >> ((b) - (int)log2(B)))

const int DATA_SIZE = sizeof(int);

/*
  Note from assignment page:
  Pay attention to the macro definition for deciding which bucket a number should be placed in:
      // bucket index for integer x of b bits (B is #buckets)
      #define BktIdx(x,b,B) ((x) >> ((b) - (int)log2(B)))
      
  For this program, data are 13-bit integers (i.e., with a value range [0, 8191]). If we use 4 buckets, then the
  leading 2 bits are used as the bucket index. For example, for the number 4734 (= 10010011111102), the
  leading 2 bits are 102, so it is to be placed in bucket[2].
*/


// Bubble sort (from bsort.c)
//
void bubble_sort(int *a, int n) {
  for (int i = 0; i < n; i++)
    for (int j = i+1; j < n; j++) 
      if (a[i] > a[j]) {
	int tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
      }
}


// Main routine 
// 
int main(int argc, char **argv) {

  int rank, nprocs, totalDataCnt;
  MPI_File fin, fout;
  MPI_Status st;
  MPI_Offset fileSize;

  /*
    1. Initialize MPI, and get the parameters, nprocs and rank. 
    Verify that nprocs is a power of 2. (If not, prompt the user and terminate.)
  */
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  if (!IsPowerOf2(nprocs)) {
    if (rank == 0)
    {
      fprintf(stderr, "nproces (# processes) must be a power of 2\n");
    }
    MPI_Finalize();
    exit(0);
  }

  /*
    2. Open the input file and get the file size. Derive the data size (i.e. parameter totalDataCnt) from the file size, and verify
    that nprocs evenly divides totalDataCnt. (If not, prompt the user and terminate.)
  */
  MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fin);   // 
  MPI_File_get_size(fin, &fileSize);
  totalDataCnt = (int)fileSize / DATA_SIZE;

  #ifdef DEBUG
    if (rank == 0)
    {
      printf("fileSize = %d bytes\n", (int)fileSize);
      printf("totalDataCnt = %d\n", totalDataCnt);
    }
  #endif

  // If nprocs does not evenly divide totalDataCnt
  if ((totalDataCnt % nprocs) != 0)
  {
    if (rank == 0)
    {
      fprintf(stderr, "Error! number of processes (%d) must evenly divide the number of data (%d) in %s\n", nprocs, totalDataCnt, argv[1]);
    }
    MPI_File_close(&fin);
    MPI_Finalize();
    exit(0);
  }

  // Handle too many processes for small data set
  if (nprocs > totalDataCnt)
  {
    nprocs = totalDataCnt;

    // Dont do anything if rank is higher than the number of data present
    if (rank > totalDataCnt)
    {
      MPI_File_close(&fin);
      MPI_Finalize();
    }
  }
  
  /*
    3. Declare a data array of size totalDataCnt/nprocs. Compute an unique file-view offset based on rank, and read 
    totalDataCnt/nprocs integers from the input file into the data array.
  */
  int myDataCnt = totalDataCnt/nprocs;        // Number of data elements this process will process
  int myBuf[myDataCnt];                  // Data array to hold my values
  int myOffset = rank * myDataCnt * DATA_SIZE;  // Value of the file-view offset for this process (in bytes)

  // Now open the file at the location of the calculated offset
  MPI_File_set_view(fin, myOffset, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  // Read the portion of data values assigned to this process into its buffer
  MPI_File_read(fin, myBuf, myDataCnt, MPI_INT, &st);
  // Close the file for reading 
  MPI_File_close(&fin); 

  #ifdef DEBUG
    printf("P%d: ", rank);
    for (int i = 0; i < myDataCnt; i++)
    {
      printf("%d,", myBuf[i]);
    }
    printf("\n");
  #endif

  /*
    4. Declare an array of P buckets of size max(2*(N/P)/P, 8). The first term is of the same idea of safe-guarding
    distribution perturbations as in the sequential version (since the average bucket size is (N/P)/P), and the
    value 8 is to guarantee a minimum size to cover some corner cases, since (N/P)/P can be 0 when N is small.
  */
  int bucketSize = 8;
  int compSize = 4*myDataCnt/nprocs;
  if (compSize > bucketSize)
  {
    bucketSize = compSize;
  }

  
  /*
    5. Distribute the data in data array to the buckets. (Hint: You may copy corresponding code from bsort.c.)

    6. (Communication) Send bucket[k] to process k. Since the buckets are of different sizes, use a sequence of
    individual send/recv routines to implement this. (It is easier than using collective routines.) You need to
    be careful in organizing your code to avoid deadlock, since every process is both a sender and a receiver.
  */

  int bucketDataCnt[nprocs];
  for (int i = 0; i < nprocs; i++)
  {
    bucketDataCnt[i] = 0;
  }

  int myBuckets[nprocs][compSize];
  for (int i = 0; i < myDataCnt; i++)
  {
    int destRank = BktIdx(myBuf[i], DATABITS, nprocs);

    myBuckets[destRank][bucketDataCnt[destRank]] = myBuf[i];
    bucketDataCnt[destRank]++;
  }

  int newDataTotal = 0;
  for (int i = 0; i < nprocs; i++)
  {
    MPI_Reduce(&bucketDataCnt[i], &newDataTotal, 1, MPI_INT, MPI_SUM, i, MPI_COMM_WORLD);
  }

  for (int i = 0; i < nprocs; i++)
  {
    for (int j = 0; j < bucketDataCnt[i]; j++)
    {
      int destRank = BktIdx(myBuckets[i][j], DATABITS, nprocs);
      #ifdef DEBUG
        printf("P%d sending %d to P%d\n", rank, myBuckets[i][j], destRank);
      #endif
      MPI_Send(&myBuckets[i][j], 1, MPI_INT, destRank, destRank, MPI_COMM_WORLD);
    }
  }

  // Wait until all have sent
  MPI_Barrier(MPI_COMM_WORLD);

  /*
    7. After receiving p-1 buckets from other processes, merge them with own bucket[rank] into a buffer (of size
    2*N/P), and sort them with bubble sort
  */
  // Get data from other threads
  int recvBuf[newDataTotal];
  for (int i = 0; i < newDataTotal; i++)
  {
    MPI_Recv(&recvBuf[i], 1, MPI_INT, MPI_ANY_SOURCE, rank, MPI_COMM_WORLD, &st);
  }

  #ifdef DEBUG
  printf("P%d before sort: ", rank);
  for (int i = 0; i < newDataTotal; i++)
  {
    printf("%d,", recvBuf[i]);
  }
  printf("\n");
  #endif

  // Sort the buckets
  bubble_sort(recvBuf, newDataTotal);

  #ifdef DEBUG
  printf("P%d after sort: ", rank);
  for (int i = 0; i < newDataTotal; i++)
  {
    printf("%d,", recvBuf[i]);
  }
  printf("\n");
  #endif

  /*
    8. Figure out a way to collectively compute all processes’ file-view offsets for writing to the output file. (Hint:
    A scan operation over bucket size should help.)
  */
  int offset = 0;
  MPI_Scan(&newDataTotal, &offset, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  offset -= newDataTotal;
  offset *= DATA_SIZE;

  /*
    9. Open the output file and write the data in sorted buffer to the proper location in the file. Note that you
    should select both the CREATE and the WRONLY modes when opening the output file. This would allow the
    file be created if it’s not already there. (Note that if the file already exists, new data will be written over
    the old data, but only for the section the new data covers, i.e. if there is more old data, part of that will
    remain.)
  */
  // Open the file for writing
  MPI_File_open(MPI_COMM_WORLD, argv[2], MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &fout);
  MPI_File_set_view(fout, offset, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  MPI_File_write(fout, recvBuf, newDataTotal, MPI_INT, &st);

  // Close the file for writing
  MPI_File_close(&fout);

  // Wrap up MPI
  MPI_Finalize();
}