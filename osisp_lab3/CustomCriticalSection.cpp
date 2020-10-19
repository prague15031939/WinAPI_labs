#include "CustomCriticalSection.h"

BOOL LockSpinlock(CUSTOM_CIRTICAL_SECTION* lpCriticalSection) {
	DWORD i = 0;
	int eax = 1;
	while (i < lpCriticalSection->SpinCount) {
		if (!lpCriticalSection->slock) {
			eax = InterlockedExchange(&lpCriticalSection->slock, eax);
			if (eax == 0)
				return TRUE;
		}
		i++;
	}
	return FALSE;
}

void UnlockSpinlock(CUSTOM_CIRTICAL_SECTION* lpCriticalSection) {
	int eax = 0;
	InterlockedExchange(&lpCriticalSection->slock, eax);
}

BOOL InitializeCustomCriticalSectionAndSpinCount(CUSTOM_CIRTICAL_SECTION* lpCriticalSection, DWORD dwSpinCount) {
	if (!lpCriticalSection->isInitialized) {
		lpCriticalSection->slock = 0;
		lpCriticalSection->SpinCount = dwSpinCount;
		lpCriticalSection->LockSemaphore = CreateMutex(NULL, false, MUTEX_NAME);
		return TRUE;
	}
	return FALSE;
}

void EnterCustomCriticalSection(CUSTOM_CIRTICAL_SECTION* lpCriticalSection) {
	//if (!LockSpinlock(lpCriticalSection))
		LockSpinlock(lpCriticalSection);
		WaitForSingleObject(lpCriticalSection->LockSemaphore, INFINITE);
		//OpenMutex(SYNCHRONIZE, FALSE, MUTEX_NAME);
	lpCriticalSection->OwningThread = GetCurrentThread();
}

void LeaveCustomCriticalSection(CUSTOM_CIRTICAL_SECTION* lpCriticalSection) {
	if (ReleaseMutex(lpCriticalSection->LockSemaphore) == 0)
		UnlockSpinlock(lpCriticalSection);
}
