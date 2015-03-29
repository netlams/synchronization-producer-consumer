#include "stdafx.h"
using namespace std;

#define NUM_THRD 5 
#define BUFF_SIZE 10

HANDLE ghMutex; // handle mutual exclusion
VOID showMemoryCells(); // need impt

class SharedObject {
	public:
	Monitor* monitor;
	SharedObject();
	DWORD memory[BUFF_SIZE];
	DWORD head;
	DWORD tail;
}sharedobj;

class Monitor {
	VOID initialize();
	CONDITION_VARIABLE ghEmpty;
	CONDITION_VARIABLE ghFull;
	VOID deposit(int);
	int retrieve();
};
	DWORD WINAPI Thrd_Producing( LPVOID);
	DWORD WINAPI Thrd_Consuming( LPVOID);

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE pThread[NUM_THRD];
	HANDLE cThread[NUM_THRD];
	DWORD ThreadID[NUM_THRD];
	int *lpArgPtr;
	
	sharedobj.monitor = (Monitor*) malloc(sizeof(Monitor));
	// INITIALIZE
	//SharedObject ;
	// Create a ghMutex with no initial owner
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed ghMutex
	/*sharedobj.tail = 0;
	head = 0;*/ // init in constructor

	/*ghEmpty = CreateSemaphore(NULL, BUFF_SIZE, BUFF_SIZE, NULL);
	ghFull = CreateSemaphore(NULL, 0, BUFF_SIZE, NULL);*/ // using cond var
	srand (time(NULL));
/*
	for (int z = 0; z < BUFF_SIZE; z++)
	{
		cout<< "[" << sharedobj.memory[z] << "]";		
	}
	cout << endl;*/
	showMemoryCells();

	if (!ghMutex)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return 1;
	} //error getting ghMutex
	/*if (!ghEmpty || !ghFull == NULL) 
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return 1;
	}*/ // check on these in monitor

	// Create worker threads

	for(int i=0; i < NUM_THRD; i++ )
	{
		lpArgPtr = (int *)malloc(sizeof(int));
		*lpArgPtr = i;
		pThread[i] = CreateThread( //obj
			NULL,       // default security attributes
			0,          // default stack size
			(LPTHREAD_START_ROUTINE) Thrd_Producing,
			lpArgPtr,       // no thread function arguments
			0,          // default creation flags
			&ThreadID[i]); // receive thread identifier
		//printf("Thread %d was created\n",ThreadID[i]);

		cThread[i] = CreateThread( //obj
			NULL,       // default security attributes
			0,          // default stack size
			(LPTHREAD_START_ROUTINE) Thrd_Consuming,
			lpArgPtr,       // no thread function arguments
			0,          // default creation flags
			&ThreadID[i]); // receive thread identifier
		//printf("Thread %d was created\n",ThreadID[i]);

		if( !pThread[i] || !cThread[i])
		{
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}//error getting thrd
	} //rof

	// Wait for all threads to terminate

	WaitForMultipleObjects(NUM_THRD, 
		pThread,
		TRUE, 
		INFINITE);
	WaitForMultipleObjects(NUM_THRD, 
		cThread,
		TRUE, 
		INFINITE);

	// Close thread and ghMutex handles

	for(int j=0; j < NUM_THRD; j++ )
	{
		CloseHandle(pThread[j]);
		CloseHandle(cThread[j]);
	}
	CloseHandle(ghMutex);
	//CloseHandle(ghEmpty);
	//CloseHandle(ghFull);
	showMemoryCells();

	return 0;
}
//////////
DWORD WINAPI Thrd_Producing ( LPVOID lpParam )
{
	int id, item;
	id = *(int *)lpParam;
	DWORD dwWaitResult;
	
	while( true )
	{
		//dwWaitResult = WaitForSingleObject( 
		//	ghEmpty,   // handle to semaphore
		//	INFINITE);           // zero-second time-out interval

		//switch (dwWaitResult)
		//{
		//	// The thread got ownership of the ghMutex
		//	case WAIT_OBJECT_0: 
		//		// Request ownership of ghMutex.
		//		dwWaitResult = WaitForSingleObject(
		//			ghMutex,    // handle to ghMutex
		//			INFINITE);  // no time-out interval
		//		switch (dwWaitResult)
		//		{
		//			// The thread got ownership of the ghMutex
		//			case WAIT_OBJECT_0:
		//				__try {
		//					memory[head] = 1;

		//					printf(" +[%d] producing [pos: %d]+\n ", a, head);
		//					for (int z = 0; z < BUFF_SIZE; z++)
		//					{
		//						if(z == head) 
		//							cout<< "[*" << memory[z] << "]";
		//						else
		//							cout<< "[" << memory[z] << "]";		
		//					}
		//					cout << endl;

		//					fflush(stdout);
		//					head = (head+1) % BUFF_SIZE;
		//					tail = (tail+1) % BUFF_SIZE;
		//					
		//				}

		//			__finally {
		//			// Release ownership of the ghMutex object
		//				if (! ReleaseMutex(ghMutex))
		//				{
		//				// Handle error.
		//				}
		//			}
		//			break;

		//		// The thread got ownership of an abandoned ghMutex
		//		// The database is in an indeterminate state
		//		case WAIT_ABANDONED:
		//			return FALSE;
		//		}

  //              // Simulate thread spending time on task
  //             // Sleep(10);

  //              // Release the semaphore when task is finished
  //              if (!ReleaseSemaphore( 
  //                      ghFull,		// handle to semaphore
  //                      1,            // increase count by one
  //                      NULL) )       // not interested in previous count
  //              {
  //                  printf("ReleaseSemaphore error: %d\n", GetLastError());
  //              }
  //              break; 
		//case WAIT_TIMEOUT: 
  //              printf("Thread %d: wait timed out\n", GetCurrentThreadId());
  //              break; 
		//}
		item = 0; //produce
		//sharedobj.monitor->deposit(item);

		WaitForSingleObject(&empty,INFINITE);
                 EnterCriticalSection(&cs);

		sharedobj.head = (sharedobj.head+1) % BUFF_SIZE;
		sharedobj.tail = (sharedobj.tail+1) % BUFF_SIZE;

		Sleep(rand() % 2000 + 1000);

	}
		return TRUE;
}

DWORD WINAPI Thrd_Consuming ( LPVOID lpParam )
{
	// lpParam not used in this example
	//UNREFERENCED_PARAMETER(lpParam);
	int a;
	a = *(int *)lpParam;
	//printf("\n[%d] Consumer \n", a);
	DWORD dwWaitResult;
	int cnt = 0;

	while( cnt < 8 )
	{
		// Request ownership of ghMutex.
		//dwWaitResult = WaitForSingleObject(
		//	ghMutex,    // handle to ghMutex
		//	INFINITE);  // no time-out interval
		cnt++;
		dwWaitResult = WaitForSingleObject( 
			ghFull,   // handle to semaphore
			INFINITE);           // zero-second time-out interval

		switch (dwWaitResult)
		{
			// The thread got ownership of the ghMutex
			case WAIT_OBJECT_0:
				// Request ownership of ghMutex.
				dwWaitResult = WaitForSingleObject(
					ghMutex,    // handle to ghMutex
					INFINITE);  // no time-out interval
				switch (dwWaitResult)
				{
					// The thread got ownership of the ghMutex
					case WAIT_OBJECT_0:
						__try {
							head = (head-1) % BUFF_SIZE;
							tail = (tail-1) % BUFF_SIZE;
							memory[tail] = 0;

							printf(" --[%d] consuming [pos:%d]-- \n ", a, tail);
							fflush(stdout);

							for (int z = 0; z < BUFF_SIZE; z++)
							{
								if(z == head) 
									cout<< "[*" << memory[z] << "]";
								else
									cout<< "[" << memory[z] << "]";		
							}
							cout<< endl;
							
						}

					__finally {
					// Release ownership of the ghMutex object
						if (! ReleaseMutex(ghMutex))
						{
						// Handle error.
						}
					}
					break;

				// The thread got ownership of an abandoned ghMutex
				// The database is in an indeterminate state
				case WAIT_ABANDONED:
					return FALSE;
				}

                // Simulate thread spending time on task
                //Sleep(10);

                // Release the semaphore when task is finished
                if (!ReleaseSemaphore( 
                        ghEmpty,		// handle to semaphore
                        1,            // increase count by one
                        NULL) )       // not interested in previous count
                {
                    printf("ReleaseSemaphore error: %d\n", GetLastError());
                }
                break; 
		case WAIT_TIMEOUT: 
                printf("Thread %d: wait timed out\n", GetCurrentThreadId());
                break; 
		}
		Sleep(rand() % 2000 + 1000);

	}
		return TRUE;
}

VOID showMemoryCells() {
}

VOID Monitor::deposit(int item) {
	
}

int Monitor::retrieve() {
	return ;
}