# synchronization-producer-consumer
<h2>Language written in: </h2>C++
<h2>Compilation Platform: </h2>Linux GNU++ Compiler for pthread API, Windows Visual Studio 2012 for Win32 API
 Implementing a solution to the producer-consumer problem (multiple producers and multiple consumers) using synchronization of concurrent threads.
<h2>Intro</h2>
<p>
  In this laboratory project I want to build solutions to the classic producer-consumer problem – also known as the bounded-buffer problem. Essentially this problem deals with situations where there are a number of producers and a number of consumers. These entities are assigned instructions to perform individually but would require accessing the shared buffer queue between all them. Producer theads are each to 'produce' an item (whatever the item is doesn't matter) and then deposit it into the an avaiable slot/cell in the memory buffer. Consumer threads each would access a slot/cell and 'consume' the item. 
</p>
<p>
  One of the biggest conflicts is to make sure that both producers and consumers stay in sync – that is, producers don’t over-produce and consumers don’t consume faster than producers. The consumers would only want to consume when there are items available (in the queue). The producers would only want to produce when there are empty slots available. To solve this, we must provide governing rules for them all to follow and make sure each group stay aware of what the state of the buffer queue is (available slot or no?). This is one of the important examples of synchronization for interprocess communication. To completely solve synchronization, the solution must satisfy all three conditions: mutual exclusion, progress, and bounded waiting. 
</p>

<p>
 	There are two parts to this project. The first part is implemented on Linux using the Pthread API. In the Linux solution, mutex and couting semaphores were used. The other part uses the Win32 API for the Window platform. This solution tries to imitate a monitor abstract datatype using CRITICAL SECTION and CONDITIONAL VARIABLE instead. Both are solutions to the producer-consumer problem and the differences are the techniques used.
</p>

<h2>Usage: </h2>
Part 1- Pthread
1. Must be compiled/executed on a Linux OS. GNU++ must be installed.
2. Using the Terminal, type "g++ -o main main.cpp -lpthread" 
*remove the quotes*
3. In the same directory, execute the binary file "main". You could type "./main"
*again remove the quotes*
4. The program will run on the Terminal, to stop running press CRTL+C
Part 2 - Win32
1. Open the solution file.
2. Build and run it in MS Visual Studio (2012)

