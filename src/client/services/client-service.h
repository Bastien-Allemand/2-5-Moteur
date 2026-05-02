#pragma once

#include <framework/services/network-service.h>
#include <framework/networking/message-listener.h>
#include <framework/networking/packet.h>
#include <utils/networking/socket.h>

#include "../messages/handshake-message.h"
#include <utils/threading/mutex.h>

class ClientService : public NetworkService {
public:
    ClientService();
    ~ClientService() override;
    
    template <typename M, typename T>
    void AddMessageListener(T* _inst, MessageListenerCallback<M, T> _callback);

    template <typename M>
    void sendMessage(M& _data);

    void connect(const std::string& _username);
    void disconnect();

    const bool isConnected() const;
    
    OwnerID getOwner() const;

protected:

    void OnStartup() override;
    void OnUpdate() override;
    void OnShutdown() override;

    void OnThreadRunning() override;
    
    // Listeners
    void OnConnected(HandshakeMessage* _message, OwnerID _owner);

private:
    UdpSocket m_socket;

    uint m_ageCounter;
    uint m_lastAge;
    bool m_isConnected;
    OwnerID m_owner;
    std::vector<MessageListenerBase*> m_listeners;
    Mutex m_listenersMutex; // Protect listeners
};
#include "client-service.inl"