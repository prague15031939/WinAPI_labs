#include "logger.h"

using namespace std;

ThreadPoolLogger::ThreadPoolLogger(std::string path) {
	filePath = path;
	fileObj.open(filePath.c_str(), ios::out | ios::app);
}

void ThreadPoolLogger::createPool(int maxCount) {
	if (fileObj.is_open()) {
		stringstream ss;
		ss.clear();
		ss << getTime() << " - INFO - " << "thread pool was created with " << maxCount << " threads" << std::endl;
		fileObj << ss.str();
	}
}

void ThreadPoolLogger::addTask(int taskNum) {
	if (fileObj.is_open()) {
		stringstream ss;
		ss.clear();
		ss << getTime() << " - INFO - " << "new task was added, ordinal = " << taskNum << std::endl;
		fileObj << ss.str();
	}
}

void ThreadPoolLogger::taskException(int threadID, const char* info) {
	if (fileObj.is_open()) {
		stringstream ss;
		ss.clear();
		ss << getTime() << " - ERROR - " << "exception emerged in thread#" << threadID << ": " << info << std::endl;
		fileObj << ss.str();
	}
}

void ThreadPoolLogger::overflowed() {
	if (fileObj.is_open()) {
		stringstream ss;
		ss.clear();
		ss << getTime() << " - WARNING - " << "pool is out of threads, task was not added" << std::endl;
		fileObj << ss.str();
	}
}

void ThreadPoolLogger::useThread(int threadID) {
	if (fileObj.is_open()) {
		stringstream ss;
		ss.clear();
		ss << getTime() << " - INFO - " << "thread #" << threadID << " accepted the task" << std::endl;
		fileObj << ss.str();
	}
}

void ThreadPoolLogger::freeThread(int threadID) {
	if (fileObj.is_open()) {
		stringstream ss;
		ss.clear();
		ss << getTime() << " - INFO - " << "thread #" << threadID << " successfully executed task" << std::endl;
		fileObj << ss.str();
	}
}
void ThreadPoolLogger::destroyThread(int threadID) {
	if (fileObj.is_open()) {
		stringstream ss;
		ss.clear();
		ss << getTime() << " - INFO - " << "thread #" << threadID << " completed" << std::endl;
		fileObj << ss.str();
	}
}

void ThreadPoolLogger::destroyPool() {
	if (fileObj.is_open()) {
		stringstream ss;
		ss.clear();
		ss << getTime() << " - INFO - " << "thread pool was destroyed" << std::endl;
		fileObj << ss.str();

		fileObj.close();
	}
}

char* ThreadPoolLogger::getTime() {
	time_t now = time(NULL);
	char* currentTime =  ctime(&now);
	currentTime[strlen(currentTime) - 1] = '\0';
	return currentTime;
}
