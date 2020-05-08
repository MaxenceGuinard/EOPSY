#include <stdio.h>
#include <stdlib.h>
#include <signal.h> //to add signal function
#include <unistd.h> //to add fork and sleep function
#include <sys/wait.h> //to add wait function
#include <sys/types.h> //to add pid_t type

//#define WITH_SIGNALS

#define NUM_CHILD 5

// array where the children pid are
int array[NUM_CHILD]; 
// count the number of children created
int cCreated = 0;
// count the number of children killed
int killed = 0; 
// flaPc = 1 mean that in handler is it the parent. if flacPc = 0 child..
int flagPc;

void handler()
{
    // case when parent call handler
    if (flagPc == 1)
    {
        printf("\nSIGTERM received from parent[%d]", getpid());
        // kill all the children process
        for (int i = 0; i < cCreated; ++i)
        {
            kill(array[i], SIGTERM);
            killed++;
        }
    }

    //case where child call handler 
    if (flagPc == 0)
    {
        printf("\nChild [%d]: SIGTERM signal received.\n", getpid());

        // kill the current child when SIGTERM is received
        kill(getpid(), SIGKILL);
        exit(EXIT_SUCCESS);
    }
}

int main()
{
    system("clear");

    pid_t pid;
    for(int i = 0; i < NUM_CHILD; i++)
    {
        pid = fork();


        if(pid >= 0)
        {       
                // Child process
                if(pid == 0)
                {
                    #ifdef WITH_SIGNALS
                    // notify than we are in one child
                    flagPc = 0;

                    signal(SIGINT, SIG_IGN);
                    signal(SIGTERM, handler);

                    #endif
                    
                    sleep(10);
                    printf("\nChild[%d]: PPID = [%d]. Process completed.", getpid(), getppid());
                    exit(EXIT_SUCCESS);
                    
                }
                // Parent process
                else
                {
                    #ifdef WITH_SIGNALS
                    // notify that we are in the parent
                    flagPc = 1;

                    // desactivate all signal 
                    for(int k = 1; k <= NSIG; k++)
                    {
                        signal(k, SIG_IGN);
                    }

                    // reactivate the SIGCHLD signal
                    signal(SIGCHLD, SIG_DFL);
                    //call handler function if SIGINT is received
                    signal(SIGINT, handler);

                    #endif
                    // store the pid of the created child
                    array[i] = pid;
                    printf("Parent[%d]: Child[%d] has been created.\n", getpid(), pid);
                    //count numer off created child
                    cCreated++;
                }
        }
        // When error occur
        else
        {   
            perror("Fork creation error!");

            // kill all processes in the current process (children) with SIGTERM
            kill(0, SIGTERM);
            exit(1);
        }

        sleep(1);
    }

    for (;;) 
    {
        if (wait(NULL) == -1) 
        {
            printf("\n\nThere are no more processes to be synchronized with the parent\n");
            #ifdef WITH_SIGNALS
                
            printf("%d children have been killed.\n", killed);

            #endif

            exit(0);
        }
    }

    // reactivate all the signal 
    for (int i = 1; i <= NSIG; i++)
    {
        signal(i, SIG_DFL);
    }
    return 0;
}
