#pragma once
#include <windows.h>
#include <stack>
#include "logger.h"

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
	BOOL exec(LPTHREAD_START_ROUTINE ThreadProc, LPVOID lpParam = NULL);

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

	ThreadPoolLogger *logger;
};

