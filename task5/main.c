#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

void *createClient(void *_id);
void *createBarber();

// Women
#define N1 0
// Men
#define N2 0
// Both
#define N3 1

// Chairs
#define M 1

// Clients
#define C 4

int barber_number = N1 + N2 + N3;
int clientAreAllDone = 0;
int tab_client[C];

sem_t waiting_room;
sem_t barber_chair;
sem_t barber_bed;
sem_t seateBealt;
// _____________________________________________________________________________________________________________________________
int main() 
{
    //randomTimeIni();

    pthread_t barberThread;
    pthread_t clientThread[C];

    // array initialisation
    for (int i = 0; i < C; i++)
    {
        tab_client[i] = i;
    }
    
    sem_init(&waiting_room, 0, M);
    sem_init(&barber_chair, 0, 1);
    sem_init(&barber_bed, 0, 0);
    sem_init(&seateBealt, 0, 0);

    // Create barber thread
    pthread_create(&barberThread, NULL, createBarber, NULL);

    for (int i = 0; i < C; i++)
    {
        pthread_create(&clientThread[i], NULL, createClient, (void *)&tab_client[i]);
        sleep(1);
    }

    for (int i = 0; i < C; i++)
    {
        pthread_join(clientThread[i], NULL);
        //sleep(1);
    }
    
    

    clientAreAllDone = 1;

    sem_post(&barber_bed);
    pthread_join(barberThread, NULL);



    return 0;
}

// _____________________________________________________________________________________________________________________________
void *createClient(void *_id)
{
    int id = *(int *)_id;
    //char sexe = returnRandomSexe();

    printf("Client %d is going to the barber shop.\n", id);
    //waitRandomTimeBetween0And(10);
    sleep(1);
    printf("Client %d arrive in the barber shop\n", id);
    
    sem_wait(&waiting_room);
    printf("Client %d take place in the waiting room\n", id);

    sem_wait(&barber_chair);

    sem_post(&waiting_room);

    printf("Client %d waking the barber up.\n", id);
    sem_post(&barber_bed);

    sem_wait(&seateBealt);
    sem_post(&barber_chair);
    printf("Client %d leaving barber shop.\n", id);
}   

// _____________________________________________________________________________________________________________________________
void *createBarber()
{
    while (!clientAreAllDone)
    {
        printf("The barber is spleeping.\n");
        sem_wait(&barber_bed);

        if (!clientAreAllDone)
        {
            printf("The barber is cutting hair\n");
            //waitRandomTimeBetween0And(2);
            sleep(1);
            printf("The barber has finished cutting hair.\n");

            sem_post(&seateBealt);
        }
        else
        {
            printf("The barber is done for today.");
        }
    }
}


/*char returnRandomSexe()
{
    int number = rand() % 1;
    if (number == 0)
    {
        return "F";
    }
    else
    {
        return "M";
    }
}

// Needed to use random values
void randomTimeIni()
{
    system("clear");
    srand(time(NULL));
}

// Sleep random time between 0 and _number
void waitRandomTimeBetween0And(int _number)
{
    int number = rand() % _number;
    sleep(number);
}*/

