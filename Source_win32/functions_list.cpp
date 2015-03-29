// SEMAPHORE
HANDLE WINAPI CreateSemaphore(
  _In_opt_  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
  _In_      LONG lInitialCount,
  _In_      LONG lMaximumCount,
  _In_opt_  LPCTSTR lpName
);
https://msdn.microsoft.com/en-us/library/windows/desktop/ms682438(v=vs.85).aspx

DWORD WINAPI WaitForMultipleObjects(
  _In_  DWORD nCount,
  _In_  const HANDLE *lpHandles,
  _In_  BOOL bWaitAll,
  _In_  DWORD dwMilliseconds
);
https://msdn.microsoft.com/en-us/library/windows/desktop/ms687025(v=vs.85).aspx

DWORD WINAPI WaitForSingleObject(
  _In_  HANDLE hHandle,
  _In_  DWORD dwMilliseconds
);
https://msdn.microsoft.com/en-us/library/windows/desktop/ms687032(v=vs.85).aspx

BOOL WINAPI ReleaseSemaphore(
  _In_       HANDLE hSemaphore,
  _In_       LONG lReleaseCount,
  _Out_opt_  LPLONG lpPreviousCount
);
https://msdn.microsoft.com/en-us/library/windows/desktop/ms685071(v=vs.85).aspx

BOOL WINAPI CloseHandle(
  _In_  HANDLE hObject
);
https://msdn.microsoft.com/en-us/library/windows/desktop/ms724211(v=vs.85).aspx

// MUTEX
HANDLE WINAPI CreateMutex(
  _In_opt_  LPSECURITY_ATTRIBUTES lpMutexAttributes,
  _In_      BOOL bInitialOwner,
  _In_opt_  LPCTSTR lpName
);
https://msdn.microsoft.com/en-us/library/windows/desktop/ms682411(v=vs.85).aspx

HANDLE WINAPI CreateThread(
  _In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
  _In_       SIZE_T dwStackSize,
  _In_       LPTHREAD_START_ROUTINE lpStartAddress,
  _In_opt_   LPVOID lpParameter,
  _In_       DWORD dwCreationFlags,
  _Out_opt_  LPDWORD lpThreadId
);
https://msdn.microsoft.com/en-us/library/windows/desktop/ms682453(v=vs.85).aspx

DWORD WINAPI WaitForSingleObject(
  _In_  HANDLE hHandle,
  _In_  DWORD dwMilliseconds
);
https://msdn.microsoft.com/en-us/library/windows/desktop/ms687032(v=vs.85).aspx

BOOL WINAPI ReleaseMutex(
  _In_  HANDLE hMutex
);
https://msdn.microsoft.com/en-us/library/windows/desktop/ms685066(v=vs.85).aspx

CRITICAL_SECTION CritSection;
CONDITION_VARIABLE ConditionVar;

void PerformOperationOnSharedData()
{ 
   EnterCriticalSection(&CritSection);

   // Wait until the predicate is TRUE

   while( TestPredicate() == FALSE )
   {
      SleepConditionVariableCS(&ConditionVar, &CritSection, INFINITE);
   }

   // The data can be changed safely because we own the critical 
   // section and the predicate is TRUE

   ChangeSharedData();

   LeaveCriticalSection(&CritSection);

   // If necessary, signal the condition variable by calling
   // WakeConditionVariable or WakeAllConditionVariable so other
   // threads can wake
}