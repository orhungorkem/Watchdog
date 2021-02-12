/** \file watchdog.c
*  \author Orhun Görkem
*  \date 26 Dec 2020
*  \n  File that specifies actions of the watchdog process.   
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <time.h>
#include <signal.h>



void generate(pid_t pidList[]);  

void handleTermination(pid_t pid, pid_t pidList[]);   

void sigterm();  


int number_of_processes;   /*!< The number of processes that watchdog should generate and keep in execution */
char watchdog_output_path[1000];   /*!< The path of output file for watchdog  */
char process_output_path[1000];   /*!< The path of output file for child processes  */
char* myfifo=(char*) "/tmp/myfifo";   /*!< The path for named pipe */
FILE* fp; /*!< The file descriptor for watchdog output file */
FILE* fp2;  /*!< The file descrpitor for process output file */
int fd;  /*!< The file descriptor for named pipe */
struct timespec delta = {0 /*secs*/, 400000000 /*nanosecs*/}; 



//! General flow for lifetime of watchdog process is managed.
/**
 * Watchdog is responsible for generating given number of processes and restarting them if any of them terminates.
 * On the other hand, watchdog passes process id's of its children to a named pipe where executor can reach. 
 * This helps executor in the sense of killing the signals to correct processes.
 * \n
 * To achieve these goals, watchdog opens the named pipe, generates the children by generate() and finally gets in an infinite loop.
 * In the loop, it waits for terminations of children by wait() system call.
 * When termination occurs handleTermination() is called where necessary actions are hold.
 * \see number_of_processes is taken as an argument
 * \see process_output_path is taken as an argument
 * \see watchdog_output_path is taken as an argument
 * \see handleTermination()
 * \see generate()
*/
int main(int argc, char ** argv){


    signal(SIGTERM, sigterm);

    number_of_processes=atoi(argv[1]);
    strcpy(process_output_path,argv[2]);
    strcpy(watchdog_output_path,argv[3]);

    
    pid_t pidList[number_of_processes+1];  // The list for mapping child process numbers to process id's 

    
    fp=fopen(watchdog_output_path,"w");
    fclose(fp);
    fp2=fopen(process_output_path,"w");   
    fclose(fp2);

    fd = open(myfifo, O_WRONLY); 
    char to_write[30];
    sprintf(to_write,"P%d %d",0,getpid());   
    write(fd,to_write,30);  
    
    

    generate(pidList);
    
    while(1){    

        pid_t pidTerm;
        pidTerm=wait(NULL);   
        handleTermination(pidTerm, pidList);
    }
    


    return 0;

}

//! Signal handler for SIGTERM
/*! 
 * Executed when SIGTERM signal is received.
 * Opens process output file and handles the signal by printing the relevant expression.
 * After that, process exits with exit() system call.
 */
void sigterm(){
    fp=fopen(watchdog_output_path,"a");
    fprintf(fp,"Watchdog is terminating gracefully\n");
    fclose(fp);
    exit(0);

}



//! The function to handle termination of the child processes.
/*! 
 * Given pid and pidList, the terminated child is detected with a for loop.
 * Detecting which child is terminated is important since if P1 (head process) is terminated, all others must also be killed and restarted.
 * If another process is terminated, the job for watchdog to do is just restarting the terminated child.
 * In addition, the termination and restarting operation must be logged to watchdog output file, then inferring which child is terminated is important
 * in case of logging the process number of the corresponding child to watchdog output file.
 * \n
 * When head process dies, watchdog kills other processes by sending SIGTERM. To preserve the order of ascending process numbers,
 * wait system call is used after each SIGTERM sent. When wait returns with the news of child is terminated, the next child is sent SIGTERM.
 * \n
 * After each child process is killed, all of them are restared by generate().
 * \param pid The process id of terminated child.
 * \param pidList The list of process id's of child processes, to map the process numbers to process id's.
 * \see generate()
 */
void handleTermination(pid_t pid,pid_t pidList[]){


    int processNum=-1;
    for(int i=1;i<=number_of_processes;i++){  
        if(pidList[i]==pid){
            processNum=i;
            break;
        }
    }

    if ( processNum==-1 || processNum==0 ){  
       return;
    }


    else if(processNum==1){  

        fp=fopen(watchdog_output_path,"a");
        fprintf(fp,"P1 is killed, all processes must be killed\nRestarting all processes\n");
        fclose(fp);

        for (int i=2; i<=number_of_processes ;i++){ 
            kill(pidList[i] , SIGTERM);
            wait(NULL);   
            
        }
        generate(pidList);  
  
    }

    else{
        
        fp=fopen(watchdog_output_path,"a");
        fprintf(fp,"P%d is killed\nRestarting P%d\n",processNum,processNum);
        fclose(fp);
        if((pid=fork())==0){        
            char i2[10];
            sprintf(i2,"%d",processNum);
            execlp("./process","./process",i2,process_output_path,NULL);    
        }

        else{  
            
            pidList[processNum]=pid;
            char to_write[30];
            sprintf(to_write,"P%d %d",processNum,pid);   
            write(fd,to_write,30);  
            fp=fopen(watchdog_output_path,"a");
            fprintf(fp,"P%d is started and it has a pid of %d\n",processNum,pid);
            fclose(fp);
        }

    }

}


//! The function to generate child processes as many as given number_of_processes.
/*! 
 * Children are generated with a for loop and sequential fork() calls.
 * nanosleep() is used with 0.2 seconds to preserve the ascending order of process numbers.
 * Process numbers are different than process id's. Process numbers are used to enumarate the children.
 * After forking each child, its process id is stored in pidList to recall afterall. 
 * The process id's are also written to the named pape where executor can read.
 * \n
 * Child processes execute process.c since their work mechanism is different. This is provided with execlp().
 * The process_output_path and process number are passed as arguments to execlp() since children will need them.
 * \param pidList The list of process id's of child processes, to map the process numbers to process id's.
 * \see number_of_processes
 * \see process_output_path
 */
void generate(pid_t pidList[]){

    for(int i=1;i<=number_of_processes;i++){
        nanosleep(&delta, &delta);
        pid_t pid;

        if((pid=fork())==0){        
              
            char i2[10];
            sprintf(i2,"%d",i);
            execlp("./process","./process",i2,process_output_path,NULL);       
        }

        else{               
            
            pidList[i]=pid;
            char to_write[30];
            sprintf(to_write,"P%d %d",i,pid);   
            write(fd,to_write,30);  
            fp=fopen(watchdog_output_path,"a");
            fprintf(fp,"P%d is started and it has a pid of %d\n",i,pid); //write to file
            fclose(fp);
        }
    }

}

