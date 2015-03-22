#ifndef _PRODUCER_CONSUMER_H_
#define _PRODUCER_CONSUMER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <semaphore.h>
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
	item = 1;
}

void Producer::produce() {
	item <<= 1; // basically double
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