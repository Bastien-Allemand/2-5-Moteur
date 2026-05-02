#include "pch.h"
#include "service.h"

Service::Service() {
	m_initialized = false;
}

Service::~Service() {
	m_initialized = false;
}

void Service::OnStartup() {
	m_initialized = true;
}

void Service::OnUpdate() {
	
}

void Service::OnShutdown() {
	
}
