#include "pch.h"
#include "mutex.h"

#include "../plateform/win-safe.h"

Mutex::Mutex() {
	m_section = new CRITICAL_SECTION();
	InitializeCriticalSection((CRITICAL_SECTION*)m_section);
}
Mutex::~Mutex() {
	DeleteCriticalSection((CRITICAL_SECTION*)m_section);
	delete m_section;
}

void Mutex::lock() {
	EnterCriticalSection((CRITICAL_SECTION*)m_section);
}

void Mutex::unlock() {
	LeaveCriticalSection((CRITICAL_SECTION*)m_section);
}