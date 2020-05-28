#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <pthread.h>
#include <semaphore.h>

void *createClient(void *_id);
void *createBarber(void * _id);

void randomTimeIni();
char returnRandomGender();
int returnBarberType();

// Women
#define N1 1
// Men
#define N2 1
// Both
#define N3 2

// Chairs
#define M 0

// Clients
#define C 2

int _N1 = N1;
int _N2 = N2;
int _N3 = N3;

int returnBarberType()
{
    if (_N1 != 0)
    {
        _N1--;
        return 0;
    }
    if (_N1 == 0 && _N2 != 0)
    {
        _N2--;
        return 1;
    }
    if (_N1 == 0 && _N2 == 0 && _N3 != 0)
    {
        _N3--;
        return 2;
    }    
}

int barber_number = N1 + N2 + N3;
bool clientAreAllDone = false;
int tab_barber[N1+N2+N3];
int tab_client[C];

sem_t waiting_room;

sem_t barber_chair_W;
sem_t barber_chair_M;
sem_t barber_chair_B;

sem_t barber_bed;
sem_t waitbarber;

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

    sem_init(&barber_chair_W, 0, N1);
    sem_init(&barber_chair_M, 0, N2);
    sem_init(&barber_chair_B, 0, N3);

    sem_init(&barber_bed, 0, 0);
    sem_init(&waitbarber, 0, 0);

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


    printf("sdf\n");
    sleep(1);

    for (int i = 0; i < C; i++)
    {
        pthread_join(clientThread[i], NULL);
        sleep(1);
    }
    printf("sdf2\n");
    
    clientAreAllDone = true;
    
    for (int i = 0; i < N1+N2+N3; i++)
    {
        sem_post(&barber_bed);
        pthread_join(barberThread[i], NULL);
        sleep(1);
    }
}

// _____________________________________________________________________________________________________________________________
void *createClient(void *_id)
{
    int id = *(int *)_id;
    char gender = returnRandomGender();
    int place_number;

    sem_getvalue(&waiting_room, &place_number);

    if (place_number == 0)
    {
        printf("Client(%c) %d give up.\n", gender, id);
    }
    else
    {
        printf("Client(%c) %d is going to the barber shop.\n", gender, id);
    
        sleep(1);
        printf("Client(%c) %d arrive in the barber shop\n", gender, id);

        sem_wait(&waiting_room);
        printf("Client(%c) %d take place in the waiting room\n", gender, id);

        if (gender == 'W')
        {
            sem_wait(&barber_chair_W);
        }
        if (gender == 'M')
        {
            sem_wait(&barber_chair_M);
        }
        

        sem_post(&waiting_room);

        printf("Client(%c) %d waking the barber up.\n", gender, id);
        sem_post(&barber_bed);

        sem_wait(&waitbarber);

        if (gender == 'W')
        {
            sem_post(&barber_chair_W);
        }
        if (gender == 'M')
        {
            sem_post(&barber_chair_M);
        }

        printf("Client(%c) %d leaving barber shop.\n", gender, id);   
    }
}   

// _____________________________________________________________________________________________________________________________
void *createBarber(void *_id)
{
    int id = *(int *)_id;
    int _type = returnBarberType();
    char type;

    switch (_type)
    {
    case 0:
        type = 'W';
        break;
    case 1:
        type = 'M';
        break;
    case 2:
        type = 'B';
        break;
    default:
        break;
    }
    
    while (!clientAreAllDone)
    {
        printf("The barber(%c) %d is spleeping.\n", type, id);
        sem_wait(&barber_bed);

        if (!clientAreAllDone)
        {
            

            printf("The barber(%c) %d is cutting hair\n", type, id);
            sleep(2);
            printf("The barber(%c) %d has finished cutting hair.\n", type, id);

            sem_post(&waitbarber);
        }
        else
        {
            printf("The barber(%c) %d is done for today.\n", type, id);
        }
    }
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
/*void waitRandomTimeBetween0And(int _number)
{
    int number = rand() % _number;
    sleep(number);
}*/
