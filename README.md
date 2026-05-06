# COSC4302_PROJECT

A systems-level project developed for COSC 4302 (Operating Systems) that implements a custom Unix shell in [C/C++]. The project explores two distinct methods of command execution: process-based concurrency and thread-based execution. 

Project Overview: This project is divided into two separate implementations to demonstrate core OS concepts:
Part 1 (Process-Based): Uses the standard fork() and execvp() system calls to create child processes for command execution.
Part 2 (Thread-Based): Replaces process cloning with pthread_create() to execute commands within the same memory space, highlighting the differences in overhead and resource sharing.

Technical Features: 
Command Parsing: Tokenizes user input to handle commands and arguments dynamically.
Process Management: Implements fork() and wait() logic to ensure the parent shell remains responsive while children execute tasks.
Concurrency via Pthreads: Utilizes the POSIX threads library to map shell commands to individual threads.
Error Handling: Manages invalid commands and execution failures to prevent shell crashes.

Motivation: The goal of this project was to gain hands-on experience with Linux System Programming. By implementing the shell twice, I was able to:
  1. Compare Execution Models: Analyze the performance and safety trade-offs between process isolation (fork) and shared memory (pthreads).
  2. Understand System Calls: Gain a deep understanding of how the OS kernel manages lifecycle events for processes and threads.
  3. Master Linux Environments: Develop, debug, and test code specifically for Linux-based platforms.
  
  
How to Run:
Prerequisites
A Linux environment (Ubuntu, WSL, etc.)
gcc or g++ compiler

Compilation:
The project consists of two separate programs:
To compile the Process-based Shell (Part 1):
gcc -o shell_fork mini_shell_fork.c

To compile the Thread-based Shell (Part 2):
gcc -o shell_thread mini_shell_pthread.c -lpthread

ExecutionRun either version by launching the generated executable
./shell_fork
or
./shell_thread
