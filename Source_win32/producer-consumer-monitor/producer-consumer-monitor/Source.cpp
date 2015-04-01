
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "stdafx.h"

#define BUFFER_SIZE 22
#define numP 7
#define numC 6

int Buffer[BUFFER_SIZE];
int front=0;
int rear=0;
int count = 0;// count the number of item in buffer

int TotalProduced=0;
int TotalConsumed=0;

CONDITION_VARIABLE empty;// empty queue  when buffer is empty, it make producer sleep
CONDITION_VARIABLE full;// full queue, when buffer is full, it make producer sleep
CRITICAL_SECTION   Lock;



DWORD WINAPI Producer(PVOID p)
{
	int pId = (int)(INT_PTR)p;

	for (int i = 0; i < 6; i++){
		int Item = pId + 10; // produce item

		EnterCriticalSection(&Lock);// get the lock

		// keep sleep when buffer is full
		while ((front+1)%BUFFER_SIZE==rear){
		SleepConditionVariableCS(&full, &Lock, INFINITE);
		}

		// critical section
		Buffer[front % BUFFER_SIZE] = Item;//store item in buffer
		front = (front+1)%BUFFER_SIZE;// front poin to the next empty slot
		TotalProduced++;
		count++;
		printf("PID:%d, Produce item %2d, front %2d, totalProducd:%2d, in buffer:%d\n", 
			pId, Item, front, TotalProduced,count);

		LeaveCriticalSection(&Lock);// release lock
		WakeConditionVariable(&empty);// wake up consumer if someone is waiting

	//	Sleep(1);
	}


	return 0;
}

DWORD WINAPI Consumer(PVOID p)
{
	int cId = (int)(INT_PTR)p;

	for (int i = 0; i < 7; i++){

		EnterCriticalSection(&Lock);//get lock

		//keep sleep when no item in buffer
		while (rear==front){
			SleepConditionVariableCS(&empty, &Lock, INFINITE);
		}

		//critical section
		int Item = Buffer[rear];// fetch item from buffer
		Buffer[rear] = 0; // after consumer used the item, I set it to 0, so if a item display 0, it is wrong
        rear = (rear + 1) % BUFFER_SIZE;// rear point to the next full slot
		TotalConsumed++;
		count--;
		printf("   CID: %d,Consume item %2d, rear %2d, totalConsumed:%2d, in buffer:%d\n",
			cId, Item, rear,TotalConsumed,count);

		LeaveCriticalSection(&Lock);// release lock
		WakeConditionVariable(&full);// wake up producer if someone is waiting

		//Sleep(1);
	}

	return 0;
}

int main(void)
{
	int i;
	DWORD id;
	HANDLE producer[numP];
	HANDLE consumer[numC];

	//initialized the Conditional variables and mutex
	InitializeConditionVariable(&empty);
	InitializeConditionVariable(&full);
	InitializeCriticalSection(&Lock);
	

	printf("Producer-Consumer Problem\n\n");
	
	//create multiple producer
	for (i = 0; i < numP; i++){
		producer[i] = CreateThread(NULL, 0, Producer, (PVOID)i, 0, &id);
	}
	
	//create multiple consumer
	for (i = 0; i < numC; i++){
		consumer[i]= CreateThread(NULL, 0, Consumer, (PVOID)i, 0, &id);
	}
	
// wait all threads finish
	WaitForMultipleObjects(numP, producer, TRUE, INFINITE);        
	WaitForMultipleObjects(numC, consumer, TRUE, INFINITE);

	printf("\nProducer & Consumer all finish!");
	getchar();
	return 0;
}
