#pragma once

#include "../service.h"
#include "utils/types.h"

class Thread;

constexpr ushort DEFAULT_NETWORK_PORT = 1888;

class NetworkService : public Service {
public:
	NetworkService(ushort _port = DEFAULT_NETWORK_PORT);
	virtual ~NetworkService();

protected:
	virtual void OnStartup() override;
	virtual void OnUpdate() override;
	virtual void OnShutdown() override;

	virtual void OnThreadRunning() = 0;

	ushort m_port;

	Thread* m_thread;

private:
	void NetworkServiceProc();
};