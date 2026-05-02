#include "pch.h"
#include "thread.h"

#include "../plateform/win-safe.h"

ulong Thread::proc(void* _ptr) {
	Thread* self = (Thread*)_ptr;
	if (self && self->m_func)
		self->m_func();
	return 0;
}

Thread::~Thread() {
	if (m_handle && m_id)
		::TerminateThread(m_handle, 0);

	if (m_handle)
		::CloseHandle(m_handle);
}

void Thread::run() {
	DWORD err = ResumeThread(m_handle);
	(void)err;
}

void Thread::suspend() {
	DWORD err = SuspendThread(m_handle);
	(void)err;
}

bool Thread::running() const {
	return m_id != 0;
}

bool Thread::join() {
	HANDLE current = GetCurrentThread();
	if (current == m_handle)
		return true;

	DWORD retval = ::WaitForSingleObject(m_handle, INFINITE);
	return retval == WAIT_OBJECT_0;
}

bool Thread::joinWithTimeout(size_t _time) {
	HANDLE current = GetCurrentThread();
	if (current == m_handle)
		return true;

	DWORD retval = ::WaitForSingleObject(m_handle, (DWORD)_time);
	return retval == WAIT_OBJECT_0;
}