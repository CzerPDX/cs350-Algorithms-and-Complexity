//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 
//
// Brooke Czerwinski
//
// Oddven sort (data parallel version) oddeven2.chpl
//
// usage: ./oddeven2 -nl [number of locales to use]

use Random;
config const DEBUG = true;    // DEBUG statements on/off
config const WORST = false;   // WORST case scenario on/off
config const N = 10;          // Number of integers to generate and sort


// Initialize array with random uint(8) values
// (except if WORST flag is on, set array to the reverse of 1..N)
proc init_array(a:[]) 
{
  // If we are doign the worst case scenario, WORST will be true
  if WORST then
  {
    // Fill the values into the array so that they go backwards
    forall i in 0..(N-1) do
    {
      a[i] = (N - i + 1): uint(8);
    }
  }
  else
  {
    // Start a random number generator
    var randStream = new RandomStream(uint(8)); 
    // Put a random number into each of the array spots
    forall i in 0..(N-1) do
    {
      a[i] = randStream.getNext();
    }
  }
  if DEBUG then
    writeln("Init: ", a); 
}

// Verify that array is sorted
// (if not sorted, show the violation pair of elements)
proc verify_array(a:[]) 
{
  // Not parallel. Original program terminates after one failure is found.
  for i in 0..(N-2) do
  {
    if (a[i] > a[i + 1]) then
    {
      writef("FAILED: a[%i]=%i, a[%i]=%i\n", i, a[i], i+1, a[i+1]);
      return;
    }
  }
  writeln(N, " element array is sorted."); 
}

// Oddeven sort
// 
proc oddeven_sort(a:[]) 
{
  var t: int = 0;  // Total number of swaps over the run of oddeven_sort
  
  for t in 1..((N+1)/2)
  {
    var halfRoundSwaps: atomic int = 0;
    // Even Phase
    forall i in 0..(N-2) by 2 do
    {
      // If left value is larger than right, swap the values
      if (a[i] > a[i+1])
      {
        a[i] <=> a[i+1];
        halfRoundSwaps.add(1);
      }
    }
    // Return early if no swaps have occurred in even phase
    if (halfRoundSwaps.read() == 0) then
      return t;
    halfRoundSwaps.write(0);
    
    // Odd Phase
    forall i in 1..(N-2) by 2 do
    {
      // If left value is larger than right, swap the values
      if (a[i] > a[i+1])
      {
        a[i] <=> a[i+1];
        halfRoundSwaps.add(1);
      }
    }
    // Break early if no swaps have occurred in odd phase
    if (halfRoundSwaps.read() == 0) then
      return t;
    halfRoundSwaps.write(0);

    // Print debug statements if DEBUG is true
    if DEBUG then
    {
      writeln("t=", t, ": ", a);
    }
  }

  // If we haven't returned early, then the t value should be (N+1)/2
  return (N+1)/2;
}

proc main() 
{
  var a: [0..(N-1)] uint(8);
  init_array(a);
  var rounds: int = oddeven_sort(a);

  // If early termination saved any work, output message to user
  if (rounds < ((N+1)/2)) then
  {
    writeln("Early termination saved ", ((N+1)/2 - rounds), " rounds!");
  }

  verify_array(a);
}
