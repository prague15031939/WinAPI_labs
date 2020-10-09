#include "thread_pool.h"

ThreadPool::ThreadPool(int maxCount) {
	
	logger = new ThreadPoolLogger("D:\\log.txt");

	this->threadMaxCount = maxCount;
	threads = new HANDLE[maxCount];
	InitializeCriticalSection(&criticalSection);
	InitializeConditionVariable(&conditionVariable);
	canAccept = true;
	isFull = false;

	for (int i = 0; i < maxCount; i++)
		threads[i] = CreateThread(NULL, 0, ThreadStart, (void*)this, 0, NULL);

	logger->createPool(maxCount);
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
			logger->destroyThread(GetCurrentThreadId());
			return 0;
		}

		Task* currentTask = tasks.top();
		tasks.pop();
		LeaveCriticalSection(&criticalSection);

		logger->useThread(GetCurrentThreadId());
		currentTask->ThreadProc(currentTask->lpParam);
		logger->freeThread(GetCurrentThreadId());

	} while (canAccept);

	logger->destroyThread(GetCurrentThreadId());
	return 0;
}

BOOL ThreadPool::exec(LPTHREAD_START_ROUTINE ThreadProc, LPVOID lpParam) {
	if (threadAmount == threadMaxCount)
		isFull = true;

	if (!isFull) {
		EnterCriticalSection(&criticalSection);
		tasks.push(new Task(ThreadProc, lpParam));
		threadAmount++;
		logger->addTask(threadAmount);
		LeaveCriticalSection(&criticalSection);

		WakeConditionVariable(&conditionVariable);
		return true;
	}
	else {
		logger->overflowed();
		return false;
	}
}

ThreadPool::~ThreadPool() {

	canAccept = false;
	WakeAllConditionVariable(&conditionVariable);
	WaitForMultipleObjects(threadAmount, threads, TRUE, INFINITE);
	DeleteCriticalSection(&criticalSection);

	for (int i = 0; i < this->threadAmount; i++) {
		CloseHandle(threads[i]);
	}

	logger->destroyPool();
}