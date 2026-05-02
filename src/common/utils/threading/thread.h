#pragma once

#include "../types.h"
#include "../plateform/win-safe.h"

#include <functional>

constexpr size_t DEFAULT_JOIN_TIMEOUT = 2000;

class Thread {
public:
	template<typename F, typename... Args>
	Thread(F&& _func, Args&&... _args);

	~Thread();

	void run();
	void suspend();

	bool running() const;

	bool join();
	bool joinWithTimeout(size_t _time = DEFAULT_JOIN_TIMEOUT);
private:
	void* m_handle;

	ulong m_id;
	ulong m_parentID;

	std::function<void()> m_func;

	static ulong proc(void* _ptr);
};

#include "thread.inl"