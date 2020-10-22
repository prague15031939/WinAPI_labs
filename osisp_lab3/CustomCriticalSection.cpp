#include "CustomCriticalSection.h"
#include <iostream>
#include <sstream>

using namespace std;

BOOL LockSpinlock(CUSTOM_CIRTICAL_SECTION* lpCriticalSection) {
	DWORD i = 0;
	int eax = 1;
	while (i < lpCriticalSection->SpinCount) {
		if (!lpCriticalSection->slock) {
			eax = InterlockedExchange(&lpCriticalSection->slock, eax);
			if (eax == 0) {
				return TRUE;
			}
		}
		i++;
	}
	stringstream ss;
	ss.clear();
	ss << GetCurrentThreadId() << " sucked on spinlock\n";
	cout << ss.str();
	return FALSE;
}

void UnlockSpinlock(CUSTOM_CIRTICAL_SECTION* lpCriticalSection) {
	int eax = 0;
	InterlockedExchange(&lpCriticalSection->slock, eax);
	stringstream ss;
	ss.clear();
	ss << GetCurrentThreadId() << " unlocked spinlock\n";
	cout << ss.str();
}

BOOL InitializeCustomCriticalSectionAndSpinCount(CUSTOM_CIRTICAL_SECTION* lpCriticalSection, DWORD dwSpinCount) {
	if (!lpCriticalSection->isInitialized) {
		lpCriticalSection->slock = 0;
		lpCriticalSection->OwningThread = 0;
		lpCriticalSection->SpinCount = dwSpinCount;
		lpCriticalSection->LockSemaphore = CreateMutex(NULL, false, MUTEX_NAME);
		return TRUE;
	}
	return FALSE;
}

void EnterCustomCriticalSection(CUSTOM_CIRTICAL_SECTION* lpCriticalSection) {
	bool isLocked = LockSpinlock(lpCriticalSection);
	WaitForSingleObject(lpCriticalSection->LockSemaphore, INFINITE);

	stringstream ss;
	ss.clear();
	if (isLocked)
		ss << GetCurrentThreadId() << " locked spinlock\n";
	else
		ss << GetCurrentThreadId() << " locked mutex\n";
	cout << ss.str();

	lpCriticalSection->OwningThread = GetCurrentThread();
}

void LeaveCustomCriticalSection(CUSTOM_CIRTICAL_SECTION* lpCriticalSection) {
	ReleaseMutex(lpCriticalSection->LockSemaphore);
	UnlockSpinlock(lpCriticalSection);
	lpCriticalSection->OwningThread = 0;
}
