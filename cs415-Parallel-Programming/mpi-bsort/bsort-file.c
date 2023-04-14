//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// bsort-file.c  -  Reads input data from a file, and writes sorted result to another file.
//
// Usage: 
//   linux> ./bsort-file B <infile> <outfile> (B must be a power of 2)
//   -- use B buckets to sort data in <infile> and write result to <outfile>
//
//    gcc bsort-file.c -o bsort-file -DDEBUG -lm       // compile with debug statements
// 
/*
  References:
  https://www.tutorialspoint.com/c_standard_library/c_function_fread.htm
  https://www.tutorialspoint.com/c_standard_library/c_function_fwrite.htm
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define DATABITS 13     // assume data are 13-bit integers: [0,8191] 

// return true if x is a power of 2
#define IsPowerOf2(x) (!((x) & ((x) - 1)))

// bucket index for integer x of b bits (B is #buckets)
#define BktIdx(x,b,B) ((x) >> ((b) - (int)log2(B)))

const int DATA_SIZE = sizeof(int);

/*
  Note from assignment page:
  Also pay attention to the macro definition for deciding which bucket a number should be placed in:
      // bucket index for integer x of b bits (B is #buckets)
      #define BktIdx(x,b,B) ((x) >> ((b) - (int)log2(B)))
      
  For this program, data are 13-bit integers (i.e., with a value range [0, 8191]). If we use 4 buckets, then the
  leading 2 bits are used as the bucket index. For example, for the number 4734 (= 10010011111102), the
  leading 2 bits are 102, so it is to be placed in bucket[2].
*/

// Print array
void print_array(int *a, int n) {
  for (int i = 0; i < n; i++)
    printf("%4d ", a[i]);
  printf("\n");
}

// Initialize array with random 13-bit int values
// (except if WORST flag is on, set array to the reverse of 1..N)
void init_array(int *a, int n) {
#ifdef WORST
  for (int i = 0; i < n; i++)
    a[i] = n - i + 1;
#else
  srand(time(NULL));
  for (int i = 0; i < n; i++)
    a[i] = rand() % 8192;
#endif  
#ifdef DEBUG
  printf("Init (%d elements): ", n);
  print_array(a, n);
#endif
}

// Verify that array is sorted (and report error if exits)
void verify_array(int *a, int n) {
  for (int i = 0; i < n-1; i++)
    if (a[i] > a[i+1]) {
      printf("FAILED: a[%d]=%d, a[%d]=%d\n", i, a[i], i+1, a[i+1]);
      return;
    }
  printf("%d element array is sorted.", n);  
}

// Bubble sort
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

// Bucket sort
//
void bucket_sort(int *a, int n, int num_buckets) {
  // allocate buckets with a safe bucket size
  int bucket[num_buckets][8*n/num_buckets]; 

  // individual bucket count
  int bcnt[num_buckets];
  for (int k = 0; k < num_buckets; k++)
    bcnt[k] = 0;

  // distribute data to buckets
  for (int i = 0; i < n; i++) {
    int k = BktIdx(a[i], DATABITS, num_buckets);
    bucket[k][bcnt[k]++] = a[i];
  }
#ifdef DEBUG
  for (int k = 0; k < num_buckets; k++) {
    printf("bucket[%d]: ", k);
    print_array(bucket[k], bcnt[k]);
  }
  printf("\n");
#endif

  // bubble sort each bucket
  for (int k = 0; k < num_buckets; k++)
    bubble_sort(bucket[k], bcnt[k]);
   
  // copy sorted data back to array a
  int i = 0;
  for (int k = 0; k < num_buckets; k++) 
    for (int j = 0; j < bcnt[k]; j++)
      a[i++] = bucket[k][j];
}

// Main routine 
// 
int main(int argc, char **argv) {

  #ifdef DEBUG
    printf("arguments: \n");
    for (int i = 1; i < argc; i++)
    {
      printf("argv[%d]: %s\n", i, argv[i]);
    }
  #endif

  if (argc != 4) {
    printf("linux> ./bsort-file B <infile> <outfile> (B must be a power of 2)\n//   -- use B buckets to sort data in <infile> and write result to <outfile>\n");
    exit(0);
  }

  int B = atoi(argv[1]);  // get param B, verify it's a power of 2
  if (!IsPowerOf2(B)) {
    printf("B (#buckets) must be a power of 2\n");
    exit(0);
  }

  
  // Open the file for reading (read only)
  FILE *fin;
  fin = fopen(argv[2], "r");
  if (fin == NULL)
  {
    fprintf(stderr, "Error! File %s is not accessible\n", argv[2]);
    exit(0);
  } 
  
  
  // The data size N is to be derived from the input file size, which can be obtained by the fseek and ftell routines:
  fseek(fin, 0L, SEEK_END);   // go to the end of file
  
  int size = ftell(fin);      // this gives the file size
  rewind(fin);                // reset the file pointer
  

  int N = size / DATA_SIZE;             // N defaults to 10
  #ifdef DEBUG
    printf("size = %d bytes\n", size);
    printf("N = %d\n", N);
    printf("\n");
  #endif

  // initialize data array
  int a[N];
  int rwCheck = 0;

  // Read data from file into array "a"
  rwCheck = fread(a, DATA_SIZE, N, fin);
  /*
    Following is the declaration for fread() function (from tutorialspoit.com):
    size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)

    Parameters:
      - ptr: This is the pointer to a block of memory with a minimum size of size*nmemb bytes.
      - size: This is the size in bytes of each element to be read.
      - nmemb: This is the number of elements, each one with a size of size bytes.
      - stream: This is the pointer to a FILE object that specifies an input stream.

      Return Value:
      The total number of elements successfully read are returned as a size_t object, which is an 
      int data type. If this number differs from the nmemb parameter, then either an error had occurred 
      or the End Of File was reached.
    
      (ref: https://www.tutorialspoint.com/c_standard_library/c_function_fread.htm)
  */
  // Close the file for readig
  fclose(fin);

  if (rwCheck != N)
  {
    fprintf(stderr, "Error! Unable to finish reading file. Exiting...\n");
    exit(0);
  }

  // Sort the data
  bucket_sort(a, N, B);

  // Open the file for writing (write only)
  FILE *fout;
  fout = fopen(argv[3], "w");
  if (fout == NULL)
  {
    fprintf(stderr, "Error occurred when trying to create %s\n", argv[3]);
    exit(0);
  } 


  rwCheck = fwrite(a, DATA_SIZE, N, fout);
  // Write the data from array "a" into otuput file
  /*
    Following is the declaration for fwrite() function. (from tutorialspoint.com):
    size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
  
    Parameters
      - ptr: This is the pointer to the array of elements to be written.
      - size: This is the size in bytes of each element to be written.
      - nmemb: This is the number of elements, each one with a size of size bytes.
      - stream: This is the pointer to a FILE object that specifies an output stream.

      Return Value:
      This function returns the total number of elements successfully returned as a size_t object, 
      which is an integral data type. If this number differs from the nmemb parameter, it will show an error.
  */
  if (rwCheck != N)
  {
    fprintf(stderr, "Error occurred when trying to write to file %s\n", argv[3]);
    exit(0);
  }

#ifdef DEBUG
  printf("Result: ");
  print_array(a, N);
#endif
  verify_array(a, N);
  printf("\n");
}