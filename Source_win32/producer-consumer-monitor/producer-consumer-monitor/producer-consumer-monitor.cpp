/////////////////////////////////////////////////////////////////////////* 
// Filename:		producer-consumer-monitor.cpp 
// Student Name:	Dau T. Lam
// Class:			CIS 3207 
// Instructor:		TA - Dawei Li / Cody Casey
// Assignment:		Lab 3 - Producer-Consumer Problem  using Win32 API (Run on Windows!)
// Date:			03/29/2015
// 
// Comments: This program will solve the producer-consumer problem using critical 
// section and conditional variables to eliminate any spurious wakeups. Multiple threads
// will be created - one group will be labeled as 'producers' will continously 'produce 
// an item to be deposited' while the other group labeled as 'consumers' will continously
// 'withdraw an item for consumption'. 
// Instructions: to run, compile this file (.cpp) and execute it using Microsoft Visual Studio
//////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
using namespace std;

#define BUFFER_SIZE 20
#define NUM_PROD 5 //set how many producer threads
#define NUM_CONS 3 //set how many consumer threads

DWORD WINAPI thrdProducing( LPVOID); //instructions for producer threads 
DWORD WINAPI thrdConsuming( LPVOID); //instrctions for consumer threads
CONDITION_VARIABLE cv_consumeble; //used to wake up blocked consumer threads
CONDITION_VARIABLE cv_produceble; //used to wake up blocked producer threads
CRITICAL_SECTION crit_section; //used to allow max one thread for mutual exclusion

/* data structure to hold shared data */
class SharedObject { 
	int head;
	int tail;
	DWORD memory[BUFFER_SIZE];
public:
	SharedObject();
	ofstream myfile;
	VOID deposit(int);
	int withdraw();
	int getHead() {return head;}
	int getTail() {return tail;}
	VOID showMemoryCells();
}sharedobject; //object constructor called - initialized

/* default constructor */
SharedObject::SharedObject() { 
	head = 0; 
	tail = 0;
	InitializeConditionVariable(&cv_consumeble);
	InitializeConditionVariable(&cv_produceble);
	InitializeCriticalSection(&crit_section);
	myfile.open ("log.txt");
}

/* put produced item to memory */
VOID SharedObject::deposit(int item) {
	memory[head%BUFFER_SIZE] = item;
	head = (head+1) % BUFFER_SIZE; //next slot
	tail = (tail+1) % BUFFER_SIZE;
}

/* get item from memory */
int SharedObject::withdraw() {
	tail = (tail-1) % BUFFER_SIZE; //next slot
	head = (head-1) % BUFFER_SIZE;
	int item = memory[tail]; //fetch item from memory
	memory[tail]=0; //consumed
	return item; //return item for consumption
}

/* output all memory cells */
VOID SharedObject::showMemoryCells() {
	char t, h;
	for (int z = 0; z < BUFFER_SIZE; z++)
	{
		cout<< "[" << memory[z] << "]";		
		myfile<<"[" << memory[z] << "]";	
	}
	cout << endl;
	myfile<<"\n";
}

/* instructions for producer threads */
DWORD WINAPI thrdProducing(LPVOID arg) {
	int pid = (int)(INT_PTR)arg;

	while(true) {
		Sleep(rand() % 2500+2000); //sleep timer
		int item = 1 + pid*pid; //item produced

		EnterCriticalSection(&crit_section); //critical section obtained

		//check if all empty slots are full then do block
		while (sharedobject.getHead()==(BUFFER_SIZE-1)) {
			SleepConditionVariableCS(&cv_produceble, &crit_section, INFINITE);
		}

		//critical section work
		printf(" ++ Producing . . . [item:%d] to [position:%d] ++\n", item, sharedobject.getHead());
		sharedobject.myfile<< " ++ Producing . . . [item:" << item << "] to [position:"<< sharedobject.getHead() << "] ++\n"  ;
		sharedobject.deposit(item);
		
		//remainder section
		sharedobject.showMemoryCells();
		cout<<endl; //newline
		LeaveCriticalSection(&crit_section); //release critical section
		WakeConditionVariable(&cv_consumeble); //wake up any waiting consumer
	}
	return 0;
}

/* instructions for consumer threads */
DWORD WINAPI thrdConsuming(PVOID arg)
{
	int cid = (int)(INT_PTR)arg;

	while(true) {
		Sleep(rand() % 2500+2000); //sleep timer
		EnterCriticalSection(&crit_section); //critical section obtained

		//check if empty then wait for producer
		while (sharedobject.getTail()==0){
			SleepConditionVariableCS(&cv_consumeble, &crit_section, INFINITE);
		}
		
		//critical section
		int item = sharedobject.withdraw();
		printf(" -- Consuming . . . [item:%d] at [position:%d] --\n", item, sharedobject.getTail());
		sharedobject.myfile<< " -- Consuming . . . [item:" << item << "] from [position:"<< sharedobject.getTail() << "] ++\n"  ;
		sharedobject.showMemoryCells();
        cout<<endl;//newline
		//remainder section
		LeaveCriticalSection(&crit_section);// release crit_section
		WakeConditionVariable(&cv_produceble);// wake up producer if someone is waiting
	}
	return 0;
}

/* MAIN FUNCTION */
int main(void)
{
	DWORD tid;
	HANDLE producer[NUM_PROD];
	HANDLE consumer[NUM_CONS];
	int *lpArgPtr;
	
	srand(time(NULL));
	printf("Starting program... printing all memory cells\n");
	sharedobject.myfile<<"Starting program... printing all memory cells\n";
	sharedobject.showMemoryCells();
	cout<<endl; //newline
	
	//initialize producer threads
	for (int i = 0; i < NUM_PROD; ++i) {
		lpArgPtr = (int *)malloc(sizeof(int));
		*lpArgPtr = i;
		producer[i] = CreateThread(NULL, 
									0, 
									(LPTHREAD_START_ROUTINE) thrdProducing, 
									(PVOID)i, 
									0, 
									&tid);
		if( !producer[i]) //error getting thread
		{
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}
	}
	
	//initialize consumer threads
	for (int j = 0; j < NUM_CONS; ++j) {
		lpArgPtr = (int *)malloc(sizeof(int));
		*lpArgPtr = j;
		consumer[j]= CreateThread(NULL, 
									0, 
									(LPTHREAD_START_ROUTINE) thrdConsuming, 
									(PVOID)j, 
									0, 
									&tid);
		if( !consumer[j]) //error getting thread
		{
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}
	}
	
	//wait for all threads to finish
	WaitForMultipleObjects(NUM_PROD, producer, TRUE, INFINITE);        
	WaitForMultipleObjects(NUM_CONS, consumer, TRUE, INFINITE);

	for(int i=0; i < NUM_PROD; ++i )
	{
		CloseHandle(producer[i]);
	}
	for(int j=0; j < NUM_CONS; ++j )
	{
		CloseHandle(consumer[j]);
	}

	DeleteCriticalSection(&crit_section);
	sharedobject.myfile.close();
	printf("\nProducer & Consumer all finish!");

	return EXIT_SUCCESS;
}


