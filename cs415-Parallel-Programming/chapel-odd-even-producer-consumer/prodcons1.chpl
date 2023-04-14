//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 
//
// Brooke Czerwinski
//
// A producer-consumer program using the cqueue module 
//
// Version 1: Base version: single producer and single consumer
//
/*
    References:
      https://chapel-lang.org/docs/primers/modules.html
*/

use cqueue;

config const numItems = 32;


proc producer() { 
  // Add items to the queue
  forall i in 1..numItems do
  {
    var buf = cqueue.add(i);
    writeln("Producer added ", i, " from buf[", buf, "]");
  }
}

proc consumer() { 
  // Remove items from the queue
  forall i in 1..numItems do
  {
    var current = cqueue.remove();
    writeln("consumer rem'd ", current[1]," from buf[", current[0], "]");
  }
}

proc main() {
  // Run the producer and consumers simultaneously
  cobegin {
    producer();
    consumer();
  }
} 
