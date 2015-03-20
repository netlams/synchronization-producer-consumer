#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
using namespace std;

class Producer {
	int item;
public:
	Producer();
	Producer(int x) : item(x) {}
	void produce();
	int retItem();
};

Producer::Producer() {
	item = 1;
}

void Producer::produce() {
	item <<= 1;
}

int Producer::retItem() {
	return item;
}

class Consumer{
	int item;
public:
	Consumer();
	Consumer(int x) : item(x) {}
	void consume(int);
	int retItem();
};

Consumer::Consumer() {
	item = 0;
}

void Consumer::consume(int *x) {
	item = *x;
	*x >>= 1;
	item >>= 1;
}

int Consumer::retItem() {
	return item;
}

