#include "client-service.h"

#pragma once

#include <utils/magic-number.h>

template <typename M, typename T>
inline void ClientService::AddMessageListener(T* _inst, MessageListenerCallback<M, T> _callback) {
    m_listenersMutex.lock(); // Protect listeners
    
    MessageListener<M, T>* listener = new MessageListener<M, T>(getMessageID<M>(), _callback, _inst);
    m_listeners.push_back(listener);
    
    m_listenersMutex.unlock();
}

template <typename M>
void ClientService::sendMessage(M& _data) {
    Message msg;
    msg.header.id = getMessageID<M>();
    ByteWriter writer{ msg.body };
    _data.serialize(writer);

    Packet packet;
    packet.header.magic_number = getMagicNumber();
    packet.header.sender = m_owner;
    packet.header.age = m_ageCounter;
    msg.toPacket(packet);
    
    packet.header.crc = crc32(packet.body.data, packet.body.count);

    RawNetworkBytes bytes;
    packet.toBytes(bytes);

    m_socket.send(bytes);
    m_ageCounter++;
}