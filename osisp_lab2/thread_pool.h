#pragma once
#include <windows.h>
#include <stack>

class Task {
public:
	Task(LPTHREAD_START_ROUTINE proc, LPVOID param) {
		ThreadProc = proc;
		lpParam = param;
	}

	LPTHREAD_START_ROUTINE ThreadProc;
	LPVOID lpParam;
};

class ThreadPool {

public:

	ThreadPool(int maxCount);
	~ThreadPool();

	BOOL exec(LPTHREAD_START_ROUTINE ThreadProc, LPVOID lpParam = NULL) {
		if (threadAmount == threadMaxCount)
			isFull = true;

		if (!isFull) {
			EnterCriticalSection(&criticalSection);
			tasks.push(new Task(ThreadProc, lpParam));
			threadAmount++;
			LeaveCriticalSection(&criticalSection);

			WakeConditionVariable(&conditionVariable);
			return true;
		}

		return false;
	}

private:
	static DWORD WINAPI ThreadStart(LPVOID lpParam);
	DWORD ThreadMain();

	CRITICAL_SECTION criticalSection;
	CONDITION_VARIABLE conditionVariable;

	bool isFull;
	bool canAccept;
	int threadMaxCount;
	int threadAmount;
	std::stack<Task*> tasks;
	HANDLE* threads;
};
