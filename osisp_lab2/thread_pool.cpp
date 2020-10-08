#include "thread_pool.h"

ThreadPool::ThreadPool(int maxCount) {

	this->threadMaxCount = maxCount;
	threads = new HANDLE[maxCount];
	InitializeCriticalSection(&criticalSection);
	InitializeConditionVariable(&conditionVariable);
	canAccept = true;
	isFull = false;

	for (int i = 0; i < maxCount; i++)
		threads[i] = CreateThread(NULL, 0, ThreadStart, (void*)this, 0, NULL);
}

DWORD WINAPI ThreadPool::ThreadStart(LPVOID lpParam) {
	return ((ThreadPool*)lpParam)->ThreadMain();
}

DWORD ThreadPool::ThreadMain() {

	do {
		EnterCriticalSection(&criticalSection);

		while (tasks.size() == 0 && canAccept)
			SleepConditionVariableCS(&conditionVariable, &criticalSection, INFINITE);

		if (!canAccept) {
			LeaveCriticalSection(&criticalSection);
			return 0;
		}

		Task* currentTask = tasks.front();
		tasks.pop_back();
		LeaveCriticalSection(&criticalSection);

		currentTask->ThreadProc(NULL);
	} while (canAccept);

	return 0;
}

ThreadPool::~ThreadPool() {

	canAccept = false;
	WakeAllConditionVariable(&conditionVariable);
	WaitForMultipleObjects(threadAmount, threads, TRUE, INFINITE);
	DeleteCriticalSection(&criticalSection);
	for (int i = 0; i < this->threadAmount; i++) {
		CloseHandle(threads[i]);
	}

}