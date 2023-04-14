//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 
//
// Brooke Czerwinski
//
// A producer-consumer program using the cqueue module 
//
// Version 2: Extended version: single producer and single consumer
//
/*
    References:
      https://chapel-lang.org/docs/primers/modules.html
*/

use cqueue;

config const numItems = 32;
config const numCons = 2;

var totalRemoved: atomic int = 0;
var queueAccess$: atomic bool = true;


proc producer() { 
  // Add items to the queue
  forall i in 1..numItems do
  {
    var buf = cqueue.add(i);
    writeln("Producer added ", i, " from buf[", buf, "]");
  }
}

proc consumer(id) { 
  // Remove items from the queue
  // while finishedAdding is false or 
  while (totalRemoved.fetchAdd(1) < numItems)
  {
    var current = cqueue.remove();
    
    writeln("consumer[", id, "] rem'd ", current[1]," from buf[", current[0], "]");
  }
}

proc main() {
  // Run the producer and consumers simultaneously
  cobegin {
    producer();
    coforall i in 1..numCons do
    {
      consumer(i);
    }
  }
} 
