/////////////////////////////////////////////////////////////////////////* 
// Filename:		pt_main.h 
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
// Instructions: to run, compile main function file (.cpp) and execute it using Terminal, See instructions.txt
//////////////////////////////////////////////////////////////////////////*/

#ifndef _PRODUCER_CONSUMER_H_
#define _PRODUCER_CONSUMER_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <bitset>

using namespace std;
//
class Producer {
	int item;
	public:
	Producer();
	Producer(int x) : item(x) {}
	void produce();
	int get_item();
};

Producer::Producer() {
	item=1;
}

void Producer::produce() {
	item <<= 1; // basically double
	// int temp = item;
	// item<<=1;
	// item|=temp;
}

int Producer::get_item() {
	return item;
}
//
class Consumer{
	int item;
	public:
	Consumer();
	void consume();
	void set_item(int);
	int get_item();
};

Consumer::Consumer() {
	item = 0;
}

void Consumer::consume() {
	item >>= 1; // basically divide 2
}

void Consumer::set_item(int x) {
	item = x;
}

int Consumer::get_item() {
	return item;
}

#endif
