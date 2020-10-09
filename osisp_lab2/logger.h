#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>

class ThreadPoolLogger
{

public:
	ThreadPoolLogger(std::string path);

	void createPool(int maxCount);
	void destroyPool();
	void overflowed();
	void addTask(int taskNum);
	void taskException(int threadID, const char* info);
	void useThread(int threadID);
	void freeThread(int threadID);
	void destroyThread(int threadID);

private:
	std::string filePath;
	std::ofstream fileObj;
	char* getTime();

};

