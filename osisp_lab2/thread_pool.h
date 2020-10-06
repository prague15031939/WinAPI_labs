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
		if (tasks.size() <= thread_max_count) {
			tasks.push_back(new Task(ThreadProc));
			thread_amount++;
			return true;
		}
		return false;
	}

private:
	static DWORD WINAPI ThreadStart(LPVOID lpParam);
	DWORD ThreadMain();

	int thread_max_count;
	int thread_amount;
	std::vector<Task*> tasks;
	HANDLE* threads;
};
