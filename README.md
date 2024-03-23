# Multi-threaded Producer-Consumer Program

This program demonstrates multi-threading concepts by implementing a Producer-Consumer problem scenario. It consists of one Producer thread and two Consumer threads. The Producer generates random integers and inserts them into a bounded integer buffer (stack), while the Consumers read from the buffer and write to different output files based on the parity of the numbers.



## Problem Statement

The program adheres to the following requirements:

- **Producer Thread:**
  - Generates integers in the range of [LOWER_NUM, UPPER_NUM].
  - Inserts each number into a bounded integer buffer (stack) of length BUFFER_SIZE.
  - Generates exactly MAX_COUNT integers, where MAX_COUNT > BUFFER_SIZE.
  - Writes all generated numbers to the output file `all.txt`.
- **Consumer Threads:**
  - Two Consumer threads are created.
  - First Consumer removes even numbers from the buffer and writes them to `even.txt`.
  - Second Consumer removes odd numbers from the buffer and writes them to `odd.txt`.
  - Consumers can only read but not remove a number with wrong parity from the buffer.
- **Program Termination:**
  - The program terminates when the Producer generates and inserts MAX_COUNT numbers to the buffer, and the Consumer threads together remove and write MAX_COUNT numbers to the corresponding files.



## Run the Program

1. Compile the program:

   ```
   gcc -o main main.c -lpthread
   ```

2. Run the program:

   ``` 
   ./main
   ```

3. To see the real, user, and sys time taken by the program:

   ``` 
   time ./main
   ```



## Program Screenshot

![screenshot](/screenshot.png)

