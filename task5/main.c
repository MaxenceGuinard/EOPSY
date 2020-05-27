#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

void *createClient(void *_id);
void *createBarber(void * _id);

void randomTimeIni();
char returnRandomGender();

// Women
#define N1 0
// Men
#define N2 0
// Both
#define N3 2

// Chairs
#define M 1

// Clients
#define C 2

int barber_number = N1 + N2 + N3;
int clientAreAllDone = 0;
int tab_barber[N1+N2+N3];
int tab_client[C];

sem_t waiting_room;

sem_t barber_chair_F;
sem_t barber_chair_M;
sem_t barber_chair_B;

sem_t barber_bed;
sem_t seateBealt;
// _____________________________________________________________________________________________________________________________
int main() 
{
    randomTimeIni();

    pthread_t barberThread[N1+N2+N3];
    pthread_t clientThread[C];

    // array initialisation
    for (int i = 0; i < N1+N2+N3; i++)
    {
        tab_barber[i] = i;
    }
    
    
    for (int i = 0; i < C; i++)
    {
        tab_client[i] = i;
    }

    
    sem_init(&waiting_room, 0, M);

    sem_init(&barber_chair_F, 0, N1);
    sem_init(&barber_chair_M, 0, N2);
    sem_init(&barber_chair_B, 0, N3);

    sem_init(&barber_bed, 0, 0);
    sem_init(&seateBealt, 0, 0);

    // Create barber threads
    for (int i = 0; i < N1+N2+N3; i++)
    {
        pthread_create(&barberThread[i], NULL, createBarber,&tab_client[i]);
    }
    
    // Create client threads
    for (int i = 0; i < C; i++)
    {
        pthread_create(&clientThread[i], NULL, createClient, &tab_client[i]);
        sleep(1);
    }

    for (int i = 0; i < C; i++)
    {
        pthread_join(clientThread[i], NULL);
        sleep(1);
    }
    
    

    clientAreAllDone = 1;

    for (int i = 0; i < N1+N2+N3; i++)
    {
        sem_post(&barber_bed);
        pthread_join(barberThread[i], NULL);
    }
    
    
    



    return 0;
}

// _____________________________________________________________________________________________________________________________
void *createClient(void *_id)
{
    int id = *(int *)_id;
    char sex = returnRandomGender();

    printf("Client %d (%c) is going to the barber shop.\n", id, sex);
    
    sleep(1);
    printf("Client %d (%c) arrive in the barber shop\n", id, sex);

    sem_wait(&waiting_room);
    printf("Client %d (%c) take place in the waiting room\n", id, sex);

    if (sex == 'F')
    {
        sem_wait(&barber_chair_F);
    }
    else
    {
        sem_wait(&barber_chair_M);
    }
    

    sem_post(&waiting_room);

    printf("Client %d (%c) waking the barber up.\n", id, sex);
    sem_post(&barber_bed);

    sem_wait(&seateBealt);
    //sem_post(&barber_chair);
    printf("Client %d (%c) leaving barber shop.\n", id, sex);
}   

// _____________________________________________________________________________________________________________________________
void *createBarber(void *_id)
{
    int id = *(int *)_id;

    while (!clientAreAllDone)
    {
        printf("The barber %d is spleeping.\n", id);
        sem_wait(&barber_bed);

        if (!clientAreAllDone)
        {
            printf("The barber %d is cutting hair\n", id);
            //waitRandomTimeBetween0And(2);
            sleep(2);
            printf("The barber %d has finished cutting hair.\n", id);

            sem_post(&seateBealt);
        }
        else
        {
            printf("The barber %d is done for today.\n", id);
        }
    }
}


char returnRandomGender()
{
    int number = rand() % 2;
    if (number == 0)
    {
        return 'F';
    }
    else
    {
        return 'M';
    }
}

// Needed to use random values
void randomTimeIni()
{
    system("clear");
    srand(time(NULL));
}

// Sleep random time between 0 and _number
/*void waitRandomTimeBetween0And(int _number)
{
    int number = rand() % _number;
    sleep(number);
}*/

