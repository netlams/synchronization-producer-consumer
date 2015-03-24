#include "main.h"

#define BUFF_SIZE 20
#define T 1

void *thrd_producing(void *arg);
void *thrd_consuming(void *arg);

unsigned int memory[BUFF_SIZE] = {0};
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
// int pos = 0;

int main(void) {
	pthread_t tid_p[1], tid_c[1];
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BUFF_SIZE);
	sem_init(&full, 0, 0);
//		cout<< "empty b4: "<<empty << endl;
//		cout<< "full b4: "<<full << endl;

	for (int i=0; i<T; i++) {
		pthread_create(&tid_p[0], NULL, thrd_producing, (void *)i);
		pthread_create(&tid_c[0], NULL, thrd_consuming, (void *)i);
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
	int i = 0;
	int pos = 0;

	do {
		
		sem_wait(&empty);
		
		p.produce();
		pthread_mutex_lock(&mutex); {
			// memory[pos] = p.return_item();
			memory[i] = p.get_item();
			printf("Producer's Thread. [pos: %d], [mem: %d]\n", i, memory[i]); fflush(stdout);
			i = (i+1) % BUFF_SIZE;
			pos++;
		}pthread_mutex_unlock(&mutex);
		sem_post(&full);

	}while(pos < BUFF_SIZE+5); // controlled loop

	return NULL;
}

void *thrd_consuming(void *arg) {
	// int pos = *((int*)&arg);	
	Consumer c;
	int i = 20;
	int pos = 0;

	do {
		
		sem_wait(&full);
		
		pthread_mutex_lock(&mutex); {
			c.set_item(memory[i]);
			c.consume(); // chg to 2
			// memory[pos] = 0;
			printf("Consumer's Thread. [pos: %d], [mem: %d], [c's: %d]\n", i, memory[i], c.get_item()); fflush(stdout);
			i = (i+1) % BUFF_SIZE;
			pos++;
		}pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}while(pos < BUFF_SIZE+5); // controlled loop

	return NULL;
}
