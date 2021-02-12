/** \file process.c
*  \author Orhun Görkem
*  \date 26 Dec 2020
*  \n File that specifies actions of processes   
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>



int p_number;  /*!< The process number denoted by watchdog (not the system process id) */
char output_path[1000];  /*!< The path of output file for processes  */
FILE* fp;    /*!< The file descrpitor for process output file */


void sighup();
void sigint();
void sigill();
void sigtrap();
void sigfpe();
void sigsegv();
void sigterm();
void sigxcpu();
void wakeup();


//! General flow for lifetime of each process is managed.
/**
 * The necessary output is printed when a process starts its execution.
 * All possible signals are waited with signal() calls to be handled.
 * Infinite loop is used to keep process in execution until SIGTERM signal is received
 * @see p_number is taken as an argument
 * @see output_path is taken as an argument
*/
int main(int argc, char ** argv){

    
    

    
    p_number=atoi(argv[1]);  
    strcpy(output_path,argv[2]);
      

    fp=fopen(output_path,"a");  
    fprintf(fp,"P%d is waiting for a signal\n",p_number);
    fclose(fp);

    signal(SIGTERM, sigterm);
    signal(SIGHUP, sighup);
    signal(SIGINT, sigint);
    signal(SIGILL, sigill);
    signal(SIGTRAP, sigtrap);
    signal(SIGFPE, sigfpe);
    signal(SIGSEGV, sigsegv);
    signal(SIGXCPU, sigxcpu);  
       
     
    for(;;);
    return 0;

}


//! Signal handler for SIGHUP
/*! 
 * Executed when SIGHUP signal is received.
 * Opens process output file and handles the signal by printing the relevant expression.
 * Here our main focus is not handling the signal in reality, we focus on interprocess communications.
 */
void sighup(){


    fp=fopen(output_path,"a");
    fprintf(fp,"P%d received signal 1\n",p_number);
    fclose(fp);
}

//! Signal handler for SIGHUP
/*! 
 * Executed when SIGHUP signal is received.
 * Opens process output file and handles the signal by printing the relevant expression.
 * Here our main focus is not handling the signal in reality, we focus on interprocess communications.
 */
void sigint(){

    fp=fopen(output_path,"a");
    fprintf(fp,"P%d received signal 2\n",p_number);
    fclose(fp);
}

//! Signal handler for SIGILL
/*! 
 * Executed when SIGILL signal is received.
 * Opens process output file and handles the signal by printing the relevant expression.
 * Here our main focus is not handling the signal in reality, we focus on interprocess communications.
 */
void sigill(){

    fp=fopen(output_path,"a");
    fprintf(fp,"P%d received signal 4\n",p_number);
    fclose(fp);
}

//! Signal handler for SIGTRAP
/*! 
 * Executed when SIGHUP signal is received.
 * Opens process output file and handles the signal by printing the relevant expression.
 * Here our main focus is not handling the signal in reality, we focus on interprocess communications.
 */
void sigtrap(){

    fp=fopen(output_path,"a");
    fprintf(fp,"P%d received signal 5\n",p_number);
    fclose(fp);
}

//! Signal handler for SIGFPE
/*! 
 * Executed when SIGHUP signal is received.
 * Opens process output file and handles the signal by printing the relevant expression.
 * Here our main focus is not handling the signal in reality, we focus on interprocess communications.
 */
void sigfpe(){

    fp=fopen(output_path,"a");
    fprintf(fp,"P%d received signal 8\n",p_number);
    fclose(fp);
}

//! Signal handler for SIGSEGV
/*! 
 * Executed when SIGHUP signal is received.
 * Opens process output file and handles the signal by printing the relevant expression.
 * Here our main focus is not handling the signal in reality, we focus on interprocess communications.
 */
void sigsegv(){
    fp=fopen(output_path,"a");
    fprintf(fp,"P%d received signal 11\n",p_number);
    fclose(fp);
}

//! Signal handler for SIGTERM
/*! 
 * Executed when SIGTERM signal is received.
 * Opens process output file and handles the signal by printing the relevant expression.
 * After that, process exits with exit() system call.
 */
void sigterm(){

    fp=fopen(output_path,"a");
    fprintf(fp,"P%d received signal 15, terminating gracefully\n",p_number);
    fclose(fp);
    exit(0);
}

//! Signal handler for SIGXCPU
/*! 
 * Executed when SIGXCPU signal is received.
 * Opens process output file and handles the signal by printing the relevant expression.
 * Here our main focus is not handling the signal in reality, we focus on interprocess communications.
 */
void sigxcpu(){

    fp=fopen(output_path,"a");
    fprintf(fp,"P%d received signal 24\n",p_number);
    fclose(fp);
}