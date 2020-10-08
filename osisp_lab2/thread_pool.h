#pragma once
#include <windows.h>
#include <vector>

class Task {
	public:
		Task(LPTHREAD_START_ROUTINE proc) {
			ThreadProc = proc;
		}

		LPTHREAD_START_ROUTINE ThreadProc;
	};

class ThreadPool {

public:

	ThreadPool(int max_count);
	~ThreadPool();

	BOOL exec(LPTHREAD_START_ROUTINE ThreadProc) {
		if (threadAmount == threadMaxCount)
			isFull = true;

		if (!isFull) {
			EnterCriticalSection(&criticalSection);
			tasks.push_back(new Task(ThreadProc));
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
	std::vector<Task*> tasks;
	HANDLE* threads;
};
