#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

#define N 5 

#define LEFT (id + N - 1) % N 
#define RIGHT (id + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

/*void grab_forks(int _id);
void put_away_forks(int _id);
void test(int _id);*/
void randomTimeIni();
void waitRandomTimeBetween0And(int _number);
void *createPhilosopher(void *_id);

int tab_forks[N];
int tab_philosopher[N];
int tab_state[N];     

pthread_t philosopherThread[N];
pthread_mutex_t forksMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s[N];
      

int main()
{
   randomTimeIni();

   for (int i = 0; i < N; i++)
   {
      tab_forks[i] = i;
      tab_philosopher[i] = i;
      tab_state[i] = i;
   }

   for (int i = 0; i < N; i++)
   {
      pthread_create(&philosopherThread[i], NULL, createPhilosopher, &tab_philosopher[i]);
   }

   for (int i = 0; i < N; i++)
   {
      pthread_join(philosopherThread[i], NULL); 
   }
}

void *createPhilosopher(void *_id)
{
   int id = *(int *)_id;
   bool locked;

   while (1)
   {
      printf("Philosopher[%d] state: THINKING\n", id);
      tab_state[id] = THINKING;
      waitRandomTimeBetween0And(10);
      printf("Philosopher[%d] state: HUNGRY\n", id);
      tab_state[id] = HUNGRY;

      locked = false;
      while (!locked)
      {
         pthread_mutex_lock(&forksMutex);
         if (tab_forks[RIGHT] == 0 || tab_forks[LEFT] == 0)
         {
            pthread_mutex_unlock(&forksMutex);
            printf("Forks are not available for philosopher %d.\nPhilosopher[%d] state: THINKING\n", id, id);
            tab_state[id] = THINKING;
            waitRandomTimeBetween0And(15);
            printf("Philosopher[%d] state: HUNGRY\n", id);
            tab_state[id] = HUNGRY;
         }
         else
         {
            // Philosopher is taking forks
            tab_forks[RIGHT] = 1;
            tab_forks[LEFT] = 1;

            pthread_mutex_unlock(&forksMutex);
            locked = true;
         }         
      }

      printf("Philosopher[%d] state: EATING.\n", id);
      tab_state[id] = EATING;
      waitRandomTimeBetween0And(5);
      //printf("Philosopher[%d] state: THINKING.\n", id);
      pthread_mutex_lock(&forksMutex);

      // Philosopher give up both forks
      tab_forks[RIGHT] = 0;
      tab_forks[LEFT] = 0;
      pthread_mutex_unlock(&forksMutex);
      waitRandomTimeBetween0And(5);
   }
}
 

/*void grab_forks(int _id) 
{
int id = _id;
  down(&forksMutex);   

  tab_state[id] = HUNGRY;   

  test(id);  

  up(&forksMutex);  

  down(&s[id]); 
}

 

void put_away_forks(int _id) 
{  
   int id = _id;
   down(&forksMutex);   
   tab_state[id] = THINKING;   
   test(LEFT);   
   test(RIGHT);  
   up(&forksMutex); 
}

 

void test(int _id) 
{  
   int id = _id;
   if(tab_state[id] == HUNGRY && tab_state[LEFT] != EATING && tab_state[RIGHT] != EATING)  
   {
      tab_state[id] = EATING;   
      up(&s[id]);
    }
 }*/

void randomTimeIni()
{
    system("clear");
    srand(time(NULL));
}

void waitRandomTimeBetween0And(int _number)
{
    //rand() % _number return the Euclidean division of the rand() number by _number
    int number = rand() % _number;
    sleep(number);
}  