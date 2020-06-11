#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

// number of philosopher
#define N 5 

// id of the philosopher on the left of the current philosopher 
#define LEFT (id + N - 1) % N

// id of the philosopher on the right of the current philosopher 
#define RIGHT (id + 1) % N

// food units
#define FU 10

// declaration of the mutex
pthread_mutex_t mutex;

// declaration of a condition
pthread_cond_t cond[N];

// declaration for philosopher threads
pthread_t philosopherThread[N];

bool flag_line_jump = false;

// array to store ohilosopher ids
int tab_philosopher[N];
int FOOD_UNITS = FU + 1;

// array to store the philosopher state
enum {THINKING, HUNGRY, EATING} tab_state[N];   

// functions declarations
void grab_forks(int _id);
void put_away_forks(int _id);
void test(int _id);
void randomTimeIni();
void waitRandomTimeBetween0And(int _number);
void *createPhilosopher(void *_id);


int main()
{
   // initialize random function
   randomTimeIni();

   // initialize the mutex
   pthread_mutex_init(&mutex, NULL);

   // initialize condition thread and philosophers id
   for (int i = 0; i < N; i++)
   {
      pthread_cond_init(&cond[i], NULL);
      tab_philosopher[i] = i;
   }

   // create all the philosopher threads 
   for (int i = 0; i < N; i++)
   {
      pthread_create(&philosopherThread[i], NULL, createPhilosopher, &tab_philosopher[i]);
      printf("Philosopher[%d] is taking place...\n", i);
   }

   printf("\n");

   // launch all the philosopher threads
   for (int i = 0; i < N; i++)
   {
      pthread_join(philosopherThread[i], NULL); 
   }

   // clean mutex
   pthread_mutex_destroy(&mutex);
   
   // clean condition threads
   for (int i = 0; i < N; i++)
   {
      pthread_cond_destroy(&cond[i]);
   }
   
   return 0;
}

void *createPhilosopher(void *_id)
{
   int id = *(int *)_id;
   int units_eaten = 0;

   sleep(1);

   while (FOOD_UNITS > 0)
   {
      printf("Philosopher[%d] state: THINKING\n", id);
      waitRandomTimeBetween0And(5);

      // lock the mutex for the current philosopher
      pthread_mutex_lock(&mutex);

         grab_forks(id);
         FOOD_UNITS--;
         if(FOOD_UNITS > 0)
            units_eaten++;
         put_away_forks(id);
      
      // unlock the mutex for the current philosopher
      pthread_mutex_unlock(&mutex);
   }
   
   sleep(10);
   
   // if loop just to create a line jump before the recap is displayed
   if(!flag_line_jump)
      printf("\n");
      flag_line_jump = true;
   
   printf("Philosopher[%d] had eaten %d food units\n", id, units_eaten);
}
 

void grab_forks(int _id) 
{
   int id = _id;
   printf("Philosopher[%d] state: HUNGRY\n", id);
   tab_state[id] = HUNGRY;   

   test(id);
}

 

void put_away_forks(int _id) 
{  
   int id = _id;
   tab_state[id] = THINKING;
   test(LEFT);
   test(RIGHT);  
}

void test(int _id) 
{  
   int id = _id;

   if(tab_state[id] == HUNGRY 
   && tab_state[LEFT] != EATING 
   && tab_state[RIGHT] != EATING
   && FOOD_UNITS > 0)
   {
      printf("Philosopher[%d] state: EATING\n", id);
      tab_state[id] = EATING;
      printf("%d units of food left...\n", FOOD_UNITS);
      waitRandomTimeBetween0And(3);
   }  
 }

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
