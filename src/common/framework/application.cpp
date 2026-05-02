#include "pch.h"
#include "application.h"

#include "service.h"
#include "networking/message-registry.h"

Application* Application::m_inst = nullptr;

Application& Application::get() {
	if (m_inst == nullptr) {
		m_inst = new Application();
	}
	return *m_inst;
}

Application::Application() {
	m_running = false;
	cpuTime.Reset();
}

Application::~Application() {
	for (Service* service : m_services) {
		delete service;
	}
	m_services.clear();
}

int Application::run() {
	m_running = true;

	for (Service* service : m_services) {
		service->OnStartup();
	}

	while (m_running) {
		if (!cpuTime.Update())
			continue;
		
		for (Service* service : m_services) {
			service->OnUpdate();
		}
	}

	for (Service* service : m_services) {
		service->OnShutdown();
	}

	if(m_inst != nullptr) {
		delete m_inst;
		m_inst = nullptr;
	}

	MessageRegistry::release();

	return 0;
}

void Application::shutdown() {
	m_running = false;
}