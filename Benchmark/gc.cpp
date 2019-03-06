#include "GC.h"

#include <chrono>

void GC::exec_routine()
{
	while (running) {
		void* trash = 0;
		if (mtx.try_lock()) {
			if (trash_queue.size())
			{
				trash = trash_queue.back();
				trash_queue.pop_back();
			}
			mtx.unlock();
		}
		if (trash)
			free(trash);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

GC::~GC()
{
	if (th)
	{
		running = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		th->detach();
		delete th;
		th = 0;
	}
}

void GC::start()
{
	if (th)
	{
		running = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		th->detach();
		delete th;
	}
	running = true;
	th = new std::thread(&GC::exec_routine, this);
}

void GC::feed(void *_tr)
{
	mtx.lock();
	trash_queue.emplace_back(_tr);
	mtx.unlock();
}