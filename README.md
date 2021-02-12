# Watchdog

This project is an implementation of a watchdog program which detects the crashing of the project components, logs the error, and then restarts that
process (or all processes of the project, if necessary). To simulate real-life scenarios, watchdog works with an executor program that sends some signals to processes 
 to handle. For simplicity, signal handlers of the processes only print which signal is received. The only signal handled is **SIGTERM**. When processes receive SIGTERM, they are terminated. The following are responsibilities of watchdog:

 * Generating child processes via `fork()`.
 * Passing process id's of child processes to executor with a **named pipe**.
 * Detecting termination of a child process and restarting it.
 * If head process of children (P0) dies, terminating all childs and restarting again.

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

## Input/Output

Input consists of some signals, destination process names, and wait commands (sleeps).
```
SIGHUP P3
wait 1
SIGINT P2
SIGTERM P3
wait 2
SIGILL P4
SIGFPE P2
wait 1
SIGTERM P1
SIGTRAP P2
wait 1
SIGTERM P5
wait 1
SIGILL P5
SIGXCPU P3
SIGSEGV P1
```

For a scenario, watchdog and child processes generate 2 output files. These files include the activities of corresponding processes.\
Watchdog output:
```
P1 is started and it has a pid of 4296
P2 is started and it has a pid of 4297
P3 is started and it has a pid of 4298
P4 is started and it has a pid of 4299
P5 is started and it has a pid of 4300
P3 is killed
Restarting P3
P3 is started and it has a pid of 4301
P1 is killed, all processes must be killed
Restarting all processes
P1 is started and it has a pid of 4302
P2 is started and it has a pid of 4303
P3 is started and it has a pid of 4304
P4 is started and it has a pid of 4305
P5 is started and it has a pid of 4306
P5 is killed
Restarting P5
P5 is started and it has a pid of 4307
Watchdog is terminating gracefully
```
Process output:
```
P1 is waiting for a signal
P2 is waiting for a signal
P3 is waiting for a signal
P4 is waiting for a signal
P5 is waiting for a signal
P3 received signal 1
P2 received signal 2
P3 received signal 15, terminating gracefully
P3 is waiting for a signal
P4 received signal 4
P2 received signal 8
P1 received signal 15, terminating gracefully
P2 received signal 15, terminating gracefully
P3 received signal 15, terminating gracefully
P4 received signal 15, terminating gracefully
P5 received signal 15, terminating gracefully
P1 is waiting for a signal
P2 is waiting for a signal
P3 is waiting for a signal
P4 is waiting for a signal
P5 is waiting for a signal
P2 received signal 5
P5 received signal 15, terminating gracefully
P5 is waiting for a signal
P5 received signal 4
P3 received signal 24
P1 received signal 11
P1 received signal 15, terminating gracefully
P2 received signal 15, terminating gracefully
P3 received signal 15, terminating gracefully
P4 received signal 15, terminating gracefully
P5 received signal 15, terminating gracefully
```


