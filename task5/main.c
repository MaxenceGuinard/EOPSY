#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

// Women
#define N1 1
// Men
#define N2 1
// Both
#define N3 1

// Chairs
#define M 5

// Customers
#define C 10

int barber_number = N1 + N2 + N3;

/*sem_t waiting_room;
sem_t barber_chair;
sem_t barber_bed;
sem_t client_sit;
*/

int main() 
{
    randomTimeIni();

    pthread_t pid[C];

    // Customer creation
    int tab_client[C];

    for (int i = 0; i < C; i++)
    {
        tab_client[i] = createCustomer
    }
    


    return 0;
}

void createCustomer(int _id)
{
    int id = _id;
    char sexe = returnRandomSexe();

    printf("Customer %d is going to the barber shop.\n", id);
    waitRandomTimeBetween0And(10);

    

}

char returnRandomSexe()
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
}

