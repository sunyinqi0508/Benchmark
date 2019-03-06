#pragma once
#include <vector>
#include <future>
#include <thread>
#include <mutex>

class GC
{
private:

	std::vector<void*> trash_queue;
	bool running = false;
	std::mutex mtx;
	void exec_routine();
	std::thread *th = 0;
public:
	GC() = default;
	~GC();
	void start();
	void feed(void*);
};
