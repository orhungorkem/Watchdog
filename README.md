# Watchdog

This project is an implementation of a watchdog program which detects the crashing of the project components, logs the error, and then restarts that
process (or all processes of the project, if necessary). To simulate real-life scenarios, watchdog works with an executor program that sends some signals to processes 
 to handle. For simplicity, signal handlers of the processes only print whiich signal is received. The only signal handled is **SIGTERM**. When processes receive SIGTERM, they are terminated. The following are responsibilities of watchdog:

 * Generating child processes via `fork()`.
 * Passing process id's of child processes to executor with a **named pipe**.
 * Detecting termination of a child process and restarting it.
 * If head process of childs(P0) dies, terminating all childs and restarting again.

 When executor has no remaining signal to send, it terminates all processes. 

 ## Compilation and Execution

First, compile executor.cpp and run in background with number of processes and path of instructions passed as arguments.
* g++ -o executor executor.cpp
*  ./executor <num_of_processes> <instructions_path> &  
<!-- end of the list -->
Then, compile watchdog.c and process.c and run watchdog. (Processes are executed with `execlp` system call.)
* gcc process.c -o process
* gcc watchdog.c -o watchdog
* ./watchdog <num_of_processes> <process_output_path> <watchdog_output_path>


