#include "pch.h"
#include "server-service.h"

#include <string>
#include <iostream>

#include <framework/application.h>
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

#include "messages/upstatus-message.h"
#include "messages/coin-message.h"
#include "messages/handshake-message.h"
#include "messages/join-message.h"
#include "objects/server-player-entity.h"
#include "utils/magic-number.h"
#include "game-service.h"

namespace {
	void print(const std::string& _str) {
		std::cout << _str << std::endl;
	}
}

ServerService::ServerService() : NetworkService(), m_socket(m_port) {
	m_lastAge = 0;
	print("Server Instantiated !");
	
	AddMessageListener<PingMessage>(this, &ServerService::OnClientPing);
}
ServerService::~ServerService() {
	
	m_listenersMutex.lock();
	for (auto* l : m_listeners) {
		delete l;
	}
	m_listeners.clear();
	m_listenersMutex.unlock();

	print("Server Obliterated !");
}

void ServerService::OnThreadRunning() {
	RawNetworkBytes bytes;
	Address address;

	m_socket.receive(bytes, &address);

	Packet recvPacket;
	recvPacket.fromBytes(bytes);

	// This packet is not sent by an official client, so we don't care about it
	if (recvPacket.header.magic_number != getMagicNumber()) {
		return;
	}
	
	// // This packet is older than previous ones, so we don't care about it's content
	// if (recvPacket.header.age < m_lastAge) {
	// 	return;
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
	
	// HANDLE CONNECTIONS (TEMPORARY)
	if (recvMsg.header.id == getMessageID<HandshakeMessage>()) {
		OnHandleConnection(*dynamic_cast<HandshakeMessage*>(recvMsgData), address);
	}
	
	// Temporary security guard for avoiding players pinging server as it restart
	if (m_owners.size() <= recvPacket.header.sender)
		return;
	
	// We are not connected / client is not registered, so we don't want to process the message
	if (!m_owners[recvPacket.header.sender]->connected)
		return;
	
	// Someone else is trying to connect on the same account from a different machine, so we don't want to process the message
	if (m_owners[recvPacket.header.sender]->address != address)
		return;
	
	m_listenersMutex.lock();
	auto listenersCopy = m_listeners;  // Copy pointers
	m_listenersMutex.unlock();

	// Process & dispatch the message
	for (MessageListenerBase* listener : listenersCopy) {
		if (listener->getMessageID() == recvMsg.header.id) { // If true, listener is listening to the exact same type of message we received
			listener->call(recvMsgData, recvPacket.header.sender); // Call the callback, passing the MessageData we received as an argument
		}
	}

	// Finally we store the age (it's inverted, lower age means the message is older) before we dispose of everything
	m_lastAge = recvPacket.header.age;
}

void ServerService::OnHandleConnection(const HandshakeMessage& _message, const Address& _address) {
	print("Received connection with username : " + _message.username);
	
	OwnerID connectionOwnerID = 0;
	
	// Credits : Julien
	// for (int i = 8; i <= 255; i++) {
	// 	if ((GetAsyncKeyState(i) & 0x8000) && !keyState[i]) {
	// 		keyState[i] = true;
	// 		if (i == VK_RETURN && !App::GetInstance().m_plData->ServerIP.empty()) {
	// 			IPDone = true;
	// 		}
	// 		else if (i == VK_BACK && !App::GetInstance().m_plData->ServerIP.empty()) {
	// 			App::GetInstance().m_plData->ServerIP.pop_back();
	// 		}
	// 		else {
	// 			char c = (char)MapVirtualKey(i, MAPVK_VK_TO_CHAR);
	// 			if (c >= 46 && c <= 57 && !IPDone) 
	// 				App::GetInstance().m_plData->ServerIP += c;
	// 		}
	// 	}
	// 	else if (!(GetAsyncKeyState(i) & 0x8000)) {
	// 		keyState[i] = false;
	// 	}

	if (m_players.contains(_message.username)) {
		// Player already exist in database
		connectionOwnerID = m_players[_message.username];
		
		// Error, we are already connected
		if (m_owners[connectionOwnerID]->connected) {
			print("Connection refused : " + _message.username + " is already connected !");
			return;
		}
		
		m_owners[connectionOwnerID]->address = _address;
	}
	else {
		// Player does not exist in database
		Owner* owner = new Owner();
		owner->username = _message.username;
		owner->address = _address;
		owner->userData = new int();
		
		int* ptr = (int*)owner->userData;
		*ptr = 0;

		owner->timeSinceLastPing = 0.0f;
		
		OwnerID ownerID = static_cast<OwnerID>(m_owners.size());
		
		m_ownersMutex.lock();
		m_owners.push_back(owner);
		m_ownersMutex.unlock();
		
		m_players[_message.username] = ownerID;
			
		connectionOwnerID = ownerID;
			
		print("Registered new owner with username : " + _message.username);
		OnPlayerConnected.Invoke(connectionOwnerID);
	}
	
	m_owners[connectionOwnerID]->connected = true;
		
	HandshakeMessage handshake_message;
	handshake_message.id = connectionOwnerID;
		
	print("Sending response to : " + _message.username);
		
	SendMessageTo<HandshakeMessage>(handshake_message, connectionOwnerID); // Connection message for client
	
	// Spawn replicated players of every connected players on target client
	for (auto it = m_players.begin(); it != m_players.end(); ++it) {
		OwnerID id = it->second;
		
		if (m_owners[id]->connected && id != connectionOwnerID) {
			PlayerJoinedMessage client_spawn_message;
			client_spawn_message.owner = id;
	
			SendMessageTo<PlayerJoinedMessage>(client_spawn_message, connectionOwnerID);

			UpStatus_Message msg;
			int score = *((int*)m_owners[id]->userData);
			uint health = 100;

			msg.username = m_owners[id]->username;
			msg.score = score;
			msg.ownerid = id;
			auto playerlist = APPLICATION.getService<GameService>().getEntities();
			for (int i = 0; i < playerlist.size(); i++)
			{
				if (ServerPlayerEntity* player = dynamic_cast<ServerPlayerEntity*>(playerlist[i]))
				{
					if (player->getOwner() == id)
					{
						health = player->getHealth();
					}
				}
			}
			msg.health = health;
			SendMessageTo<UpStatus_Message>(msg, connectionOwnerID);
		}
	}
	
	// Spawn coin for the client that just connected (bastien i hate you it's disgusting but since you want this shit i have no choice
	CoinSpawnMessage coinspawn_message;
	SendMessageTo<CoinSpawnMessage>(coinspawn_message, connectionOwnerID);
	
	print("Broadcasting spawn message !");
	
	PlayerJoinedMessage spawn_message;
	spawn_message.owner = connectionOwnerID;
	
	BroadcastMessage(spawn_message); // Spawn replicated player on every client
}
void ServerService::OnClientPing(PingMessage* _message, OwnerID _owner) {
	m_ownersMutex.lock();
	
	if (!_message->disconnected) {
		m_owners[_owner]->timeSinceLastPing = 0.0f;
	}
	else {
		m_owners[_owner]->connected = false;
		m_owners[_owner]->timeSinceLastPing -1.0f;
		std::cout << m_owners[_owner]->username + " as been disconnected ! \n";
	}

	m_ownersMutex.unlock();
}

Owner* ServerService::getOwner(OwnerID _id) {
	if(m_owners.size() <= _id) {
		return nullptr;
	}
	return m_owners[_id];
}
Owner* ServerService::getOwner(const std::string& _username) {
	for(Owner* owner : m_owners) {
		if (owner->username == _username) {
			return owner;
		}
	}
	return nullptr;
}

void ServerService::OnStartup() {
	NetworkService::OnStartup();
	print("Starting Server..");
}
void ServerService::OnUpdate() {
	NetworkService::OnUpdate();

	// Update the time since last ping for all owners (clients)
	m_ownersMutex.lock();
	for (Owner* owner : m_owners) {
		if (owner->connected) {
			// Disconect client
			if (owner->timeSinceLastPing > 10.0f) { // In seconds
				std::cout << owner->username + " as been disconnected ! \n";
				owner->timeSinceLastPing = -1;
				owner->connected = false;
			}
			owner->timeSinceLastPing += cpuTime.delta;
		}
	}
	m_ownersMutex.unlock();
}
void ServerService::OnShutdown() {
	NetworkService::OnShutdown();
	print("Stopping Server..");
}