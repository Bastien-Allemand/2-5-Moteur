#include "pch.h"
#include "network-service.h"

#include "utils/networking/net-init.h"
#include "utils/threading/thread.h"

NetworkService::NetworkService(ushort _port) : Service() {
	netStart();

	m_port = _port;
	m_thread = new Thread(&NetworkService::NetworkServiceProc, this);
}
NetworkService::~NetworkService() {
	delete m_thread;
	m_thread = nullptr;
	netShutdown();
}

void NetworkService::OnStartup() {
	m_thread->run();
}
void NetworkService::OnUpdate() {

}
void NetworkService::OnShutdown() {
	m_thread->joinWithTimeout(100);
}
void NetworkService::NetworkServiceProc() {
	while (m_thread->running()) {
		OnThreadRunning();
	}
	int a = 0;
}