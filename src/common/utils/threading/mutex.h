#pragma once

class Mutex {
public:
	Mutex();
	~Mutex();
	
	Mutex(const Mutex&) = delete;
	Mutex& operator=(const Mutex&) = delete;

	void lock();
	void unlock();
private:
	void* m_section;
};