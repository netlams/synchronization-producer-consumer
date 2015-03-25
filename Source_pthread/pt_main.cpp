#include "pt_main.h"

#define BUFF_SIZE 20
#define NUM_THRD 1

void *thrd_producing(void *arg);
void *thrd_consuming(void *arg);

unsigned int memory[BUFF_SIZE] = {0};
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int head;
int tail;

int main(void) {
	pthread_t tid_p[NUM_THRD], tid_c[NUM_THRD];
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BUFF_SIZE);
	sem_init(&full, 0, 0);
	head = tail = 0;

	srand (time(NULL));

	for (int i=0; i<NUM_THRD; i++) {
		pthread_create(&tid_p[0], NULL, thrd_producing, NULL);
		pthread_create(&tid_c[0], NULL, thrd_consuming, NULL);
	}
	for (int i=0; i<NUM_THRD; i++) {
		pthread_join(tid_p[0], NULL);
		pthread_join(tid_c[0], NULL);
	}

	pthread_mutex_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);
	pthread_exit(NULL);
}

void *thrd_producing(void *arg) {
	int id = *((int*)&arg);
	Producer p;

	do {
		sem_wait(&empty);

		p.produce();
		pthread_mutex_lock(&mutex); {
			memory[head] = p.get_item();
			printf("Producer's Thread. [pos: %d], [mem: %d]\n", head, memory[head]);
			fflush(stdout);
			head = (head+1) % BUFF_SIZE;
			tail = (tail+1) % BUFF_SIZE;
		}pthread_mutex_unlock(&mutex);
		sem_post(&full);

		sleep(rand() % 2);
	}while(true); // controlled loop

	return NULL;
}

void *thrd_consuming(void *arg) {
	int id = *((int*)&arg);
	Consumer c;

	do {
		sem_wait(&full);

		pthread_mutex_lock(&mutex); {
			tail = (tail-1) % BUFF_SIZE;
			head = (head-1) % BUFF_SIZE;
			c.set_item(memory[tail]);
			c.consume(); // halve
		 	printf("Consumer's Thread. [pos: %d], [mem: %d], ", tail, memory[tail]);

			memory[tail] = c.get_item(); // update item in buffer[slot]
			printf("after [mem: %d]\n", memory[tail]);
			fflush(stdout);
		}pthread_mutex_unlock(&mutex);
		sem_post(&empty);

		sleep(rand() % 2);
	}while(true); // controlled loop

	return NULL;
}
