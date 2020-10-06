#include <iostream>
#include <ctime>
#include "thread_pool.h"

DWORD WINAPI ThreadProc(LPVOID lpParam) {
    std::cout << time(NULL) << "\n";
    return 0;
}

int main()
{
    int counter = 0;
    ThreadPool* pool = new ThreadPool(5);
    for (int i = 0; i < 8; i++) {
        if (!pool->exec(ThreadProc))
            counter++;
    }
    Sleep(1000);
    std::cout << counter << std::endl;
}
