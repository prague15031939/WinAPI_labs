#include <iostream>
#include "thread_pool.h"

DWORD WINAPI ThreadProc(LPVOID lpParam) {
    Sleep(2000);
    std::cout << GetCurrentThreadId() << "\n";
    return 0;
}

int main()
{
    ThreadPool* pool = new ThreadPool(5);
    for (int i = 0; i < 8; i++) {
        if (!pool->exec(ThreadProc))
            std::cout << "no threads\n";
    }

    char c;
    std::cin >> c;
}
