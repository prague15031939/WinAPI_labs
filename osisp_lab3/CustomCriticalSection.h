#pragma once

#include <windows.h>

#define MUTEX_NAME LPCWSTR("old_mutex")

typedef struct CUSTOM_CIRTICAL_SECTION {
	LONG LockCount;
	LONG RecursionCount;
	HANDLE OwningThread;
	HANDLE LockSemaphore;
	ULONG_PTR SpinCount;
	alignas(4) LONG slock;

	BOOL isInitialized = FALSE;
};

void InitializeCustomCriticalSection(CUSTOM_CIRTICAL_SECTION* lpCriticalSection);
void EnterCustomCriticalSection(CUSTOM_CIRTICAL_SECTION* lpCriticalSection);
void LeaveCustomCriticalSection(CUSTOM_CIRTICAL_SECTION* lpCriticalSection);
BOOL InitializeCustomCriticalSectionAndSpinCount(CUSTOM_CIRTICAL_SECTION* lpCriticalSection, DWORD dwSpinCount);