#include "main.h"
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define BUFF_SIZE 20
#define T 1

void *thrd_producing(void *arg);
void *thrd_consuming(void *arg);

unsigned int memory[BUFF_SIZE] = {0};
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int head;
int tail;
// int pos = 0;

int main(void) {
	pthread_t tid_p[1], tid_c[1];
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BUFF_SIZE);
	sem_init(&full, 0, 0);
	head = 0;
	tail = 0;
	srand (time(NULL));

		// cout<< "empty b4: "<<empty << endl;
		// cout<< "full b4: "<<full << endl;

	for (int i=0; i<T; i++) {
		pthread_create(&tid_p[0], NULL, thrd_producing, NULL);
		pthread_create(&tid_c[0], NULL, thrd_consuming, NULL);
	}
	for (int i=0; i<T; i++) {
		pthread_join(tid_p[0], NULL);
		pthread_join(tid_c[0], NULL);
	}

	pthread_mutex_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);
	pthread_exit(NULL);
	// return EXIT_SUCCESS;
	// exit(0);
}

void *thrd_producing(void *arg) {
	// int pos = *((int*)&arg);
	Producer p;
	// int i = full;
	int pos = 0;

	do {

		sem_wait(&empty);

		p.produce();
		pthread_mutex_lock(&mutex); {
			// memory[pos] = p.return_item();
			memory[head] = p.get_item();
			printf("Producer's Thread. [pos: %d], [mem: %d]\n", head, memory[head]);
			fflush(stdout);
			head = (head+1) % BUFF_SIZE;
			tail = (tail+1) % BUFF_SIZE;
			// if (head < 0)
			// 	head = -head;

			pos++;
		}pthread_mutex_unlock(&mutex);
		sem_post(&full);

		sleep(rand() % 2);
	// }while(pos < BUFF_SIZE+2); // controlled loop
	}while(true); // controlled loop

	return NULL;
}

void *thrd_consuming(void *arg) {
	// int pos = *((int*)&arg);
	Consumer c;
	// int i = 20;
	int pos = 0;

	do {

		sem_wait(&full);

		pthread_mutex_lock(&mutex); {
			tail = (tail-1) % BUFF_SIZE;
			head = (head-1) % BUFF_SIZE;
			// if (tail < 0)
			// 	tail = -tail;
			c.set_item(memory[tail]);
			c.consume(); // chg to 2
			// printf("Consumer's Thread. [pos: %d], [mem: %d], ", tail, memory[tail], c.get_item());

			memory[tail] = c.get_item();
			printf("Consumer's Thread. [pos: %d], [mem: %d], [c's: %d]\n", tail, memory[tail], c.get_item());
			fflush(stdout);

			pos++;
		}pthread_mutex_unlock(&mutex);
		sem_post(&empty);

		sleep(rand() % 2);
	// }while(pos < BUFF_SIZE+2); // controlled loop
	}while(true); // controlled loop

	return NULL;
}
