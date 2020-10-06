#include "thread_pool.h"

ThreadPool::ThreadPool(int max_count) {
	this->thread_max_count = max_count;
	threads = new HANDLE[max_count];

	for (int i = 0; i < max_count; i++)
		threads[i] = CreateThread(NULL, 0, ThreadStart, (void*)this, 0, NULL);
}

DWORD WINAPI ThreadPool::ThreadStart(LPVOID lpParam) {
	return ((ThreadPool*)lpParam)->ThreadMain();
}

DWORD ThreadPool::ThreadMain() {

}

ThreadPool::~ThreadPool() {
	WaitForMultipleObjects(thread_amount, threads, TRUE, INFINITE);
	for (int i = 0; i < this->thread_amount; i++) {
		CloseHandle(threads[i]);
	}
}