/////////////////////////////////////////////////////////////////////////*
// Filename:		pt_main.cpp
// Student Name:	Dau T. Lam
// Class:			CIS 3207
// Instructor:		TA - Dawei Li / Cody Casey
// Assignment:		Lab 3 - Producer-Consumer Problem using PTHREAD API (Run on Linux!)
// Date:			03/28/2015
//
// Comments: This program will solve the producer-consumer problem using mutex and semaphores. Multiple threads
// will be created - one group will be labeled as 'producers' will continously 'produce
// an item to be deposited' while the other group labeled as 'consumers' will continously
// 'withdraw an item for consumption'.
// Instructions: to run, compile this file (.cpp) and execute it using Terminal, See instructions.txt
//////////////////////////////////////////////////////////////////////////*/

#include "pt_main.h"

#define BUFF_SIZE 20
#define NUM_THRD 4 //same amount of producer/consumer threads

//prototypes
void *thrd_producing(void *arg);
void *thrd_consuming(void *arg);

//global variables
unsigned int memory[BUFF_SIZE] = {0};
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int head;
int tail;
ofstream myfile;

int main(void) {
	//initialization
	pthread_t tid_p[NUM_THRD], tid_c[NUM_THRD];
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BUFF_SIZE);
	sem_init(&full, 0, 0);
	head = tail = 0;
	srand (time(NULL));
	myfile.open("log.txt");

	//startup
	cout<<"Starting program... printing all memory cells\n";
	myfile<<"Starting program... printing all memory cells\n";
	showMemoryCells(0);
	cout<<endl;

	//thread creation
	for (int i=0; i<NUM_THRD; i++) {
		pthread_create(&tid_p[0], NULL, thrd_producing, NULL);
		pthread_create(&tid_c[0], NULL, thrd_consuming, NULL);
	}
	for (int i=0; i<NUM_THRD; i++) {
		pthread_join(tid_p[0], NULL);
		pthread_join(tid_c[0], NULL);
	}

	//termination
	pthread_mutex_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);
	pthread_exit(NULL);
	myfile.close();
}

void *thrd_producing(void *arg) {
	int id = *((int*)&arg);
	Producer p;

	do {
		sem_wait(&empty);

		p.produce();
		pthread_mutex_lock(&mutex); {
			memory[head] = p.get_item();
			printf(" ++ Producing ... at [position: %d], [item: %d] ++\n", head, memory[head]);
			showMemoryCells(head);
			fflush(stdout);
			head = (head+1) % BUFF_SIZE;
			// tail = (tail+1) % BUFF_SIZE;
		}pthread_mutex_unlock(&mutex);
		sem_post(&full);
		while(tail-head == 1) {
			;}
		sleep(rand() % 2);
	}while(true);

	return NULL;
}

void *thrd_consuming(void *arg) {
	int id = *((int*)&arg);
	Consumer c;

	do {
		sem_wait(&full);

		pthread_mutex_lock(&mutex); {
			// head = (head-1) % BUFF_SIZE;
			c.set_item(memory[tail]);
			c.consume(); // halve
		 	printf(" -- Consuming ... from [position: %d], [item: %d], ", tail, memory[tail]);
			memory[tail] = c.get_item(); // update item in buffer[slot]
			printf("after [item: %d]\n", memory[tail]);
			tail = (tail+1) % BUFF_SIZE;
			showMemoryCells(tail);
			fflush(stdout);
		}pthread_mutex_unlock(&mutex);
		sem_post(&empty);
		while(head == tail) {
			;}
		sleep(rand() % 2);
	}while(true);

	return NULL;
}

void showMemoryCells(int pos) {
	for (int z = 0; z < BUFF_SIZE; z++)
	{
		if(z == pos) {
			cout<< "[*" << memory[z] << "]";
			myfile<< "[*" << memory[z] << "]";
		}
		else{
			cout<< "[" << memory[z] << "]";
			myfile<< "[" << memory[z] << "]";
		}
	}
			myfile<< "\n";
			cout<< endl;
}
