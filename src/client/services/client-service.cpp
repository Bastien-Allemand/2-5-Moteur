#include "pch.h"
#include "client-service.h"

#include <string>
#include <iostream>

#include <framework/application.h>
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

#include "messages/handshake-message.h"
#include "messages/ping-message.h"
#include "utils/magic-number.h"

namespace {
    void print(const std::string& _str) {
        std::cout << _str << std::endl;
    }
}

ClientService::ClientService() : NetworkService(), m_socket("127.0.0.1", m_port) {
    m_lastAge = 0;
    m_isConnected = false;
    m_owner = 0;
    
    AddMessageListener<HandshakeMessage>(this, &ClientService::OnConnected);
    
    print("Client Instantiated !");
}
ClientService::~ClientService() {
    print("Client Obliterated !");
    for (MessageListenerBase* listener : m_listeners) {
        delete listener;
    }
}

void ClientService::connect(const std::string& _username) {
    print("Asking server for connection..");
    HandshakeMessage data;
    data.username = _username;

    sendMessage(data);
}
void ClientService::disconnect() {
    if (!isConnected())
        return;
    
    PingMessage ping;
    ping.disconnected = true;
    
    sendMessage(ping);
    
    m_isConnected = false;
}

const bool ClientService::isConnected() const {
    return m_isConnected;
}

OwnerID ClientService::getOwner() const {
    return m_owner;
}

void ClientService::OnStartup() {
    NetworkService::OnStartup();
    print("Starting Client..");
}
void ClientService::OnUpdate() {
    NetworkService::OnUpdate();
    
    if (!m_isConnected) {
        return;
    }

    constexpr float timer = 5.0f;
    
    static float time = 0.0f;
    
    if (time < timer) {
        time += cpuTime.delta;
    }
    else {
        PingMessage ping;
        ping.disconnected = false;
    
        sendMessage(ping);
        
        time = 0.0f;
    }
}
void ClientService::OnShutdown() {
    NetworkService::OnShutdown();
    disconnect();
    print("Stopping Client..");
}

void ClientService::OnThreadRunning() {
    static std::string ip;
    if (ip.empty()) {
        std::cout << "Enter IP : ";
        std::cin >> ip;
        if (ip == "localhost") { ip = "127.0.0.1"; }
        Address address(ip.c_str(), m_port);
        m_socket.setRemote(address);
    }
    
    static std::string username;
    if (username.empty()) {
        std::cout << "Enter Username : ";
        std::cin >> username;
        connect(username);
    }
    
    RawNetworkBytes bytes;
    Address address;

    m_socket.receive(bytes, &address);

    Packet recvPacket;
    recvPacket.fromBytes(bytes);

    // This packet is not sent by an official server, so we don't care about it
    if (recvPacket.header.magic_number != getMagicNumber()) {
        return;
    }

    // // This packet is older than previous ones, so we don't care about it's content
    // if (recvPacket.header.age < m_lastAge) {
    //     return;
    // }

    // Bytes have been lost or corrupted in the packet, so we don't care about it
    if (recvPacket.header.crc != crc32(recvPacket.body.data, recvPacket.body.count)) {
        return;
    }

    Message recvMsg;
    recvMsg.fromPacket(recvPacket);

    MessageRegistry& registry = MessageRegistry::get();
    MessageData* recvMsgData = registry.create(recvMsg.header.id);
    ByteReader reader{ recvMsg.body };
    recvMsgData->deserialize(reader);
    
    m_listenersMutex.lock();
    auto listenersCopy = m_listeners;  // Copy pointers
    m_listenersMutex.unlock();

    for (MessageListenerBase* listener : listenersCopy) {
        if (listener->getMessageID() == recvMsg.header.id) { // If true, listener is listening to the exact same type of message we received
            listener->call(recvMsgData, recvPacket.header.sender); // Call the callback, passing the MessageData we received as an argument
        }
    }

	delete recvMsgData;

    // Finaly we store the age (it's inverted, lower age means the message is older) before we dispose of everything
    m_lastAge = recvPacket.header.age;
}

void ClientService::OnConnected(HandshakeMessage* _message, OwnerID _owner) {
    m_isConnected = true;
    m_owner = _message->id;
    print("Server approved us !");
}