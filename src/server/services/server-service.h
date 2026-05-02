#pragma once

#include "../messages/handshake-message.h"

#include <framework/services/network-service.h>
#include <framework/networking/message-listener.h>
#include <framework/networking/packet.h>
#include <utils/networking/socket.h>

#include <utils/event.hpp>
#include <utils/threading/mutex.h>

#include "messages/ping-message.h"

class ServerService : public NetworkService {
public:
	ServerService();
	~ServerService() override;

	template <typename M, typename T>
	void AddMessageListener(T* _inst, MessageListenerCallback<M, T> _callback);
	
	gce::Event<OwnerID> OnPlayerConnected;

	template <typename M>
	void SendMessageTo(M& _data, Address& _address);

	template <typename M>
	void SendMessageTo(M& _data, OwnerID _owner);

	template <typename M>
	void BroadcastMessage(M& _data);

	Owner* getOwner(OwnerID _id);
	Owner* getOwner(const std::string& _username);

protected:
	void OnStartup() override;
	void OnUpdate() override;
	void OnShutdown() override;
	void OnThreadRunning() override;
	
	void OnHandleConnection(const HandshakeMessage& _message, const Address& _address);
	
	// Listeners
	void OnClientPing(PingMessage* _message, OwnerID _owner);

private:
	UdpSocket m_socket;

	std::vector<MessageListenerBase*> m_listeners;
	Mutex m_listenersMutex; // Protect listeners

	std::vector<Owner*> m_owners;
	Mutex m_ownersMutex;

	uint m_ageCounter;
	uint m_lastAge;
	std::unordered_map<std::string, OwnerID> m_players; // Index to m_owners by username

};
#include "server-service.inl"