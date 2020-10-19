#include <iostream>
#include <vector>
#include <sstream>
#include "thread_pool.h"

using namespace std;

DWORD WINAPI ThreadProc1(LPVOID lpParam) {
    if (lpParam != NULL) {
        stringstream ss;
        ss << GetCurrentThreadId() << ": " << (char*)lpParam << " - string\n";
        cout << ss.str();
    }
    return 0;
}

DWORD WINAPI ThreadProc2(LPVOID lpParam) {
    if (lpParam != NULL) {
        stringstream ss;
        ss << GetCurrentThreadId() << ": " << *((int*)lpParam) << " - int\n";
        cout << ss.str();
    }
    return 0;
}

DWORD WINAPI ThreadProc3(LPVOID lpParam) {
    if (lpParam != NULL) {
        stringstream ss;
        ss << GetCurrentThreadId() << ": " << *((double*)lpParam) << " - double\n";
        cout << ss.str();
    }
    return 0;
}

int main()
{
    ThreadPool* pool = new ThreadPool(7);

    const char* itemList[6] = { "first", "second", "third", "item", "item", "item" };

    for (int i = 0; i < 10; i++) {
        const char* item = "nuts";
        if (i < 6) item = itemList[i];
        if (!pool->exec(ThreadProc1, (void*)item))
            std::cout << "no threads\n";
    }

    int i = 2;
    if (!pool->exec(ThreadProc2, &i))
        std::cout << "no threads\n";
    double j = 42.13;
    if (!pool->exec(ThreadProc3, &j))
        std::cout << "no threads\n";

    char ch;
    std::cin >> ch;
    pool->~ThreadPool();
}
