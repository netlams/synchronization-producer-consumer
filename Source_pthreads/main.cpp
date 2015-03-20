#include "main.h"

int main(void) {
	Producer pd;
	Consumer cs;
	int buffer;
	pd.produce();
	buffer = pd.retItem(); //buf

	cout<< "Produced: " << buffer << endl;
	cs.consume(&buffer);
	
	cout<< "hiyaa" << endl;
  return EXIT_SUCCESS;
}
