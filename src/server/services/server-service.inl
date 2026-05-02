#include "server-service.h"
#include "utils/magic-number.h"

#pragma once

template <typename M, typename T>
inline void ServerService::AddMessageListener(T* _inst, MessageListenerCallback<M, T> _callback) {
	m_listenersMutex.lock(); // Protect listeners
	
	MessageListener<M, T>* listener = new MessageListener<M, T>(getMessageID<M>(), _callback, _inst);
	m_listeners.push_back(listener);
	
	m_listenersMutex.unlock();
}

template<typename M>
inline void ServerService::SendMessageTo(M& _data, Address& _address) {
	Message msg;
	msg.header.id = getMessageID<M>();
	ByteWriter writer{ msg.body };
	_data.serialize(writer);

	Packet packet;
	packet.header.magic_number = getMagicNumber();
	packet.header.sender = 0; // We don't know the sender
	packet.header.age = m_ageCounter;
	msg.toPacket(packet);
	
	packet.header.crc = crc32(packet.body.data, packet.body.count);

	RawNetworkBytes bytes;
	packet.toBytes(bytes);

	m_socket.sendTo(_address, bytes);
	m_ageCounter++;
}
template<typename M>
inline void ServerService::SendMessageTo(M& _data, OwnerID _owner) {
	Owner* owner = getOwner(_owner);
	
	if (!owner) return;
	
	SendMessageTo<M>(_data, owner->address);
}

template<typename M>
inline void ServerService::BroadcastMessage(M& _data) {
	for (const auto& owner : m_owners) {
		SendMessageTo<M>(_data, owner->address);
	}
}