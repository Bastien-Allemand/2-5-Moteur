#pragma once

#include "thread.h"

template<typename F, typename... Args>
inline Thread::Thread(F&& _func, Args&&... _args) {
	m_func = std::bind(std::forward<F>(_func), std::forward<Args>(_args)...);

	m_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)proc, (LPVOID)this, CREATE_SUSPENDED, (DWORD*)&m_id);
	m_parentID = GetCurrentThreadId();
}