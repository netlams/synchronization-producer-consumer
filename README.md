# synchronization-producer-consumer
 Implementing a solution to the producer-consumer problem (multiple producers and multiple consumers) using synchronization of concurrent threads.
<p>
  In this laboratory project I want to build solutions to the classic producer-consumer problem – also known as the bounded-buffer problem. Essentially this problem deals with situations where there are a number of producers and a number of consumers. These entities are assigned instructions to perform individually but would require accessing the shared buffer queue between all them. Producer theads are each to 'produce' an item (whatever the item is doesn't matter) and then deposit it into the an avaiable slot/cell in the memory buffer. Consumer threads each would access a slot/cell and 'consume' the item. 
</p>
<p>
  One of the biggest conflicts is to make sure that both producers and consumers stay in sync – that is, producers don’t over-produce and consumers don’t consume faster than producers. The consumers would only want to consume when there are items available (in the queue). The producers would only want to produce when there are empty slots available. To solve this, we must provide governing rules for them all to follow and make sure each group stay aware of what the state of the buffer queue is (available slot or no?). This is one of the important examples of synchronization for interprocess communication. To completely solve synchronization, the solution must satisfy all three conditions: mutual exclusion, progress, and bounded waiting. 
</p>

<p>
 	There are two parts to this project. The first part is implemented on Linux using the Pthread API. In the Linux solution, mutex and couting semaphores were used. The other part uses the Win32 API for the Window platform. This solution tries to imitate a monitor abstract datatype using CRITICAL SECTION and CONDITIONAL VARIABLE instead. Both are solutions to the producer-consumer problem and the differences are the techniques used.
</p>
