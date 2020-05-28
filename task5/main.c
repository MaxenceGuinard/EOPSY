#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

// Women
#define N1 1
// Men
#define N2 1
// Both
#define N3 0

// Chairs
#define M 4

// Clients
#define C 10

// Function declaration
void *createClient(void *_id);
void *createBarberW(void * _id);
void *createBarberM(void * _id);
void *createBarberB(void * _id);
void randomTimeIni();
char returnRandomGender();
void waitRandomTimeBetween0And(int _number);

// Flag to  know when all the clients have been served
bool clientAreAllDone = false;


// Array declaration
int tab_barberW[N1];
int tab_barberM[N2];
int tab_barberB[N3];
int tab_client[C];

// Semaphore declaration
sem_t waiting_room;
sem_t barber_chair_W;
sem_t barber_chair_M;
sem_t barber_chair_B;
sem_t barber_bed_W;
sem_t barber_bed_M;
sem_t barber_bed_B;
sem_t waitbarber;

//_____________________________________________________________________________________________________________________________________________________________________________________________________________
int main() 
{
    // Initialize random function
    randomTimeIni();

    // Thread declaration
    pthread_t barberThreadW[N1];
    pthread_t barberThreadM[N2];
    pthread_t barberThreadB[N3];
    pthread_t clientThread[C];

    // Array initialisation (value = line number)
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

    // Semaphore initialization
    sem_init(&waiting_room, 0, M);
    sem_init(&barber_chair_W, 0, N1);
    sem_init(&barber_chair_M, 0, N2);
    sem_init(&barber_chair_B, 0, N3);
    sem_init(&barber_bed_W, 0, 0);
    sem_init(&barber_bed_M, 0, 0);
    sem_init(&barber_bed_B, 0, 0);
    sem_init(&waitbarber, 0, 0);

    // Thread initialization
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

    // Client
    for (int i = 0; i < C; i++)
    {
        pthread_create(&clientThread[i], NULL, createClient, &tab_client[i]);
    }


    // Launch all the client threads 
    for (int i = 0; i < C; i++)
    {
        pthread_join(clientThread[i], NULL);
    }

    // Flag to  know when all the clients have been served
    clientAreAllDone = true;

    // Display that barbers are done for today
    // Barber for Women 
    for (int i = 0; i < N1; i++)
    {
        sem_post(&barber_bed_W);
        pthread_join(barberThreadW[i], NULL);
    }

    // Barber for Men
    for (int i = 0; i < N2; i++)
    {
        sem_post(&barber_bed_M);
        pthread_join(barberThreadM[i], NULL);
    }

    // Barber for both Women and Men
    for (int i = 0; i < N3; i++)
    {
        sem_post(&barber_bed_B);
        pthread_join(barberThreadB[i], NULL);
    }
}

//_____________________________________________________________________________________________________________________________________________________________________________________________________________
void *createClient(void *_id)
{
    int id = *(int *)_id;
    char gender = returnRandomGender();

    // Variable to store place available in the waiting room
    int place_available;

    // Variable to store how many barber chair for women are available
    int w;

    // Variable to store how many barber chair for men are available
    int m;

    // Variable to store how many barber chair for both women and men are available
    int b;

    // Variable to know if the selected barber is versatile or not
    bool is_barber_versatile;
    
    waitRandomTimeBetween0And(30);


    printf("--> Client(%c) %d  arrive in the barber shop\n", gender, id);

    sem_getvalue(&waiting_room, &place_available);


    // Case where there is no place in the waiting room
    if (place_available == 0)
    {
        printf("<-- Client(%c) %d leave because there is no barber available.\n", gender, id);
    }
    else
    {   
        printf("--- Client(%c) %d  take place in the waiting room\n", gender, id);
        sem_wait(&waiting_room);

        // Case where client in a woman
        if (gender == 'W')
        {
            do
            {
                sem_getvalue(&barber_chair_W, &w);

                // Try if there is a barber for women available
                if (w > 0)
                {
                    sem_wait(&barber_chair_W);
                    is_barber_versatile = false;

                    sem_post(&waiting_room);

                    printf("--- Client(%c) %d  waking the barber up.\n", gender, id);
                    sem_post(&barber_bed_W);
                }
                else
                {
                    sem_getvalue(&barber_chair_B, &b);
                    
                    // Try if there is a versatile barber
                    if (b > 0)
                    {
                        sem_wait(&barber_chair_B);
                        is_barber_versatile = true;

                        sem_post(&waiting_room);

                        printf("--- Client(%c) %d  waking the barber up.\n", gender, id);
                        sem_post(&barber_bed_B);
                    }
                }

            } while (w == 0 && b == 0);        

        }
        
        // Case where client is a man
        if (gender == 'M')
        {
            do
            {
                sem_getvalue(&barber_chair_M, &m);  

                // Try if there is a barber for men available
                if (m > 0)
                {
                    sem_wait(&barber_chair_M);
                    is_barber_versatile = false;

                    sem_post(&waiting_room);

                    printf("--- Client(%c) %d  waking the barber up.\n", gender, id);
                    sem_post(&barber_bed_M);
                }
                else
                {
                    sem_getvalue(&barber_chair_B, &b);

                    // Try if there is a versatile barber
                    if (b > 0)
                    {
                        sem_wait(&barber_chair_B);
                        is_barber_versatile = true;
                        
                        sem_post(&waiting_room);

                        printf("--- Client(%c) %d  waking the barber up.\n", gender, id);
                        sem_post(&barber_bed_B);
                    }
                }

            } while (m == 0 && b == 0);
        }

        sem_wait(&waitbarber);
        
        // Case where le barber was versatile
        if (is_barber_versatile)
        {
            sem_post(&barber_chair_B);
        }
        else
        {   
            // case where the barber was a barber for women
            if (gender == 'W')
            {
                sem_post(&barber_chair_W);
            }

            // case where the barber was a barber for men
            if (gender == 'M')
            {
                sem_post(&barber_chair_M);
            }
        }
        printf("<-- Client(%c) %d  leaving barber shop.\n", gender, id);
    }
}   

//_____________________________________________________________________________________________________________________________________________________________________________________________________________
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
            waitRandomTimeBetween0And(15);
            printf("The barber(%c) %d has finished cutting hair.\n", type, id);

            sem_post(&waitbarber);
        }
    }
    printf("The barber(%c) %d is done for today.\n", type, id);
    sem_post(&barber_bed_W);
}

//_____________________________________________________________________________________________________________________________________________________________________________________________________________
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
            waitRandomTimeBetween0And(15);
            printf("The barber(%c) %d has finished cutting hair.\n", type, id);

            sem_post(&waitbarber);
        }
    }
    printf("The barber(%c) %d is done for today.\n", type, id);
    sem_post(&barber_bed_M);
}

//_____________________________________________________________________________________________________________________________________________________________________________________________________________
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
            waitRandomTimeBetween0And(15);
            printf("The barber(%c) %d has finished cutting hair.\n", type, id);

            sem_post(&waitbarber);
        }
    }
    printf("The barber(%c) %d is done for today.\n", type, id);
    sem_post(&barber_bed_B);
}

//_____________________________________________________________________________________________________________________________________________________________________________________________________________
char returnRandomGender()
{
    // rand() % 2 return the Euclidean division of the rand() number by 2
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

//_____________________________________________________________________________________________________________________________________________________________________________________________________________

// Needed to use random values
void randomTimeIni()
{
    system("clear");
    srand(time(NULL));
}

//_____________________________________________________________________________________________________________________________________________________________________________________________________________

// Sleep random time between 0 and _number
void waitRandomTimeBetween0And(int _number)
{
    //rand() % _number return the Euclidean division of the rand() number by _number
    int number = rand() % _number;
    sleep(number);
}

