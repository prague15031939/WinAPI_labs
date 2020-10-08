#include <iostream>
#include <vector>
#include "thread_pool.h"

DWORD WINAPI ThreadProc(LPVOID lpParam) {
    if (lpParam != NULL)
        std::cout << GetCurrentThreadId() << (char*)lpParam << "\n";
    return 0;
}

int main()
{
    ThreadPool* pool = new ThreadPool(8);

    const char* itemList[6] = { "first", "second", "third", "item", "item", "item" };

    for (int i = 0; i < 10; i++) {
        const char* item = "nuts";
        if (i < 6) item = itemList[i];
        if (!pool->exec(ThreadProc, (void*)item))
            std::cout << "no threads\n";
    }

    char ch;
    std::cin >> ch;
}
