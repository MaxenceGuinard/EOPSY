#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <pthread.h>
#include <semaphore.h>

void *createClient(void *_id);
void *createBarberW(void * _id);
void *createBarberM(void * _id);
void *createBarberB(void * _id);

void randomTimeIni();
char returnRandomGender();
void waitRandomTimeBetween0And(int _number);

// Women
#define N1 1
// Men
#define N2 1
// Both
#define N3 2

// Chairs
#define M 5

// Clients
#define C 4

bool clientAreAllDone = false;

int tab_barberW[N1];
int tab_barberM[N2];
int tab_barberB[N3];

int tab_client[C];

sem_t waiting_room;

sem_t barber_chair_W;
sem_t barber_chair_M;
sem_t barber_chair_B;

sem_t barber_bed_W;
sem_t barber_bed_M;
sem_t barber_bed_B;

sem_t waitbarber;
// _____________________________________________________________________________________________________________________________
int main() 
{
    randomTimeIni();

    pthread_t barberThreadW[N1];
    pthread_t barberThreadM[N2];
    pthread_t barberThreadB[N3];

    pthread_t clientThread[C];

    // Table initialisation
    // Barber for Women
    for (int i = 0; i < N1; i++)
    {
        tab_barberW[i] = i;
    }
    // Barber for Men
    for (int i = 0; i < N2; i++)
    {
        tab_barberM[i] = i;
    }
    // Barber for both Women and Men
    for (int i = 0; i < N3; i++)
    {
        tab_barberB[i] = i;
    }
    // Client
    for (int i = 0; i < C; i++)
    {
        tab_client[i] = i;
    }

    
    sem_init(&waiting_room, 0, M);

    sem_init(&barber_chair_W, 0, N1);
    sem_init(&barber_chair_M, 0, N2);
    sem_init(&barber_chair_B, 0, N3);

    sem_init(&barber_bed_W, 0, 0);
    sem_init(&barber_bed_M, 0, 0);
    sem_init(&barber_bed_B, 0, 0);

    sem_init(&waitbarber, 0, 0);

    // Create barber threads
    // Barber for Women 
    for (int i = 0; i < N1; i++)
    {
        pthread_create(&barberThreadW[i], NULL, createBarberW, &tab_barberW[i]);
    }
    // Barber for Men
    for (int i = 0; i < N2; i++)
    {
        pthread_create(&barberThreadM[i], NULL, createBarberM, &tab_barberM[i]);
    }
    // Barber for both Women and Men
    for (int i = 0; i < N3; i++)
    {
        pthread_create(&barberThreadB[i], NULL, createBarberB, &tab_barberB[i]);
    }
    
    sleep(1);

    // Create client threads
    for (int i = 0; i < C; i++)
    {
        pthread_create(&clientThread[i], NULL, createClient, &tab_client[i]);
    }

    for (int i = 0; i < C; i++)
    {
        pthread_join(clientThread[i], NULL);
    }
    
    

    clientAreAllDone = true;

    for (int i = 0; i < N1; i++)
    {
        sem_post(&barber_bed_W);
        pthread_join(barberThreadW[i], NULL);
    }
    for (int i = 0; i < N2; i++)
    {
        sem_post(&barber_bed_M);
        pthread_join(barberThreadM[i], NULL);
    }
    for (int i = 0; i < N3; i++)
    {
        sem_post(&barber_bed_B);
        pthread_join(barberThreadB[i], NULL);
    }
}

// _____________________________________________________________________________________________________________________________
void *createClient(void *_id)
{
    int id = *(int *)_id;
    char gender = returnRandomGender();

    int place_available;

    int w;
    int m;
    int b;
    bool is_barber_versatile;

    //printf("Client(%c) %d  is going to the barber shop.\n", gender, id);
    
    waitRandomTimeBetween0And(30);

    printf("Client(%c) %d  arrive in the barber shop\n", gender, id);

    sem_getvalue(&waiting_room, &place_available);

    if (place_available == 0)
    {
        printf("Client(%c) %d leave because there is no barber available.\n", gender, id);
    }
    else
    {   
        printf("Client(%c) %d  take place in the waiting room\n", gender, id);
        sem_wait(&waiting_room);
      
        if (gender == 'W')
        {
            do
            {
                sem_getvalue(&barber_chair_W, &w);
        
                if (w > 0)
                {
                    sem_wait(&barber_chair_W);
                    is_barber_versatile = false;

                    sem_post(&waiting_room);

                    printf("Client(%c) %d  waking the barber up.\n", gender, id);
                    sem_post(&barber_bed_W);
                }
                else
                {
                    sem_getvalue(&barber_chair_B, &b);

                    if (b > 0)
                    {
                        sem_wait(&barber_chair_B);
                        is_barber_versatile = true;

                        sem_post(&waiting_room);

                        printf("Client(%c) %d  waking the barber up.\n", gender, id);
                        sem_post(&barber_bed_B);
                    }
                }

            } while (w == 0 && b == 0);        

        }
        
        if (gender == 'M')
        {
            do
            {
                sem_getvalue(&barber_chair_M, &m);  

                if (m > 0)
                {
                    sem_wait(&barber_chair_M);
                    is_barber_versatile = false;

                    sem_post(&waiting_room);

                    printf("Client(%c) %d  waking the barber up.\n", gender, id);
                    sem_post(&barber_bed_M);
                }
                else
                {
                    sem_getvalue(&barber_chair_B, &b);

                    if (b > 0)
                    {
                        sem_wait(&barber_chair_B);
                        is_barber_versatile = true;
                        
                        sem_post(&waiting_room);

                        printf("Client(%c) %d  waking the barber up.\n", gender, id);
                        sem_post(&barber_bed_B);
                    }
                }

            } while (m == 0 && b == 0);
        }

        sem_wait(&waitbarber);
        
        if (is_barber_versatile)
        {
            sem_post(&barber_chair_B);
        }

        if (gender == 'W')
        {
            sem_post(&barber_chair_W);
        }

        if (gender == 'M')
        {
            sem_post(&barber_chair_M);
        }
        
        printf("Client(%c) %d  leaving barber shop.\n", gender, id);
    }
}   

// _____________________________________________________________________________________________________________________________
void *createBarberW(void *_id)
{
    int id = *(int *)_id;
    char type = 'W';

    while (!clientAreAllDone)
    {
        printf("The barber(%c) %d is spleeping.\n", type, id);
        sem_wait(&barber_bed_W);

        if (!clientAreAllDone)
        {
            printf("The barber(%c) %d is cutting hair\n", type, id);
            waitRandomTimeBetween0And(10);
            printf("The barber(%c) %d has finished cutting hair.\n", type, id);

            sem_post(&waitbarber);
        }
    }
    printf("The barber(%c) %d is done for today.\n", type, id);
    sem_post(&barber_bed_W);
}

void *createBarberM(void *_id)
{
    int id = *(int *)_id;
    char type = 'M';

    while (!clientAreAllDone)
    {
        printf("The barber(%c) %d is spleeping.\n", type, id);
        sem_wait(&barber_bed_M);

        if (!clientAreAllDone)
        {
            printf("The barber(%c) %d is cutting hair\n", type, id);
            waitRandomTimeBetween0And(10);
            printf("The barber(%c) %d has finished cutting hair.\n", type, id);

            sem_post(&waitbarber);
        }
    }
    printf("The barber(%c) %d is done for today.\n", type, id);
    sem_post(&barber_bed_M);
}

void *createBarberB(void *_id)
{
    int id = *(int *)_id;
    char type = 'B';

   while (!clientAreAllDone)
    {
        printf("The barber(%c) %d is spleeping.\n", type, id);
        sem_wait(&barber_bed_B);

        if (!clientAreAllDone)
        {
            printf("The barber(%c) %d is cutting hair\n", type, id);
            waitRandomTimeBetween0And(10);
            printf("The barber(%c) %d has finished cutting hair.\n", type, id);

            sem_post(&waitbarber);
        }
    }
    printf("The barber(%c) %d is done for today.\n", type, id);
    sem_post(&barber_bed_B);
}


char returnRandomGender()
{
    int number = rand() % 2;
    if (number == 0)
    {
        return 'W';
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
void waitRandomTimeBetween0And(int _number)
{
    int number = rand() % _number;
    sleep(number);
}

