#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are server side
struct HandshakeMessage : public MessageData {
	std::string username; // Client to server
	OwnerID id; // Server to client
	
	inline bool isValid() const override {
		return true;
	}

	inline void serialize(ByteWriter& _writer) override {
		_writer.write<OwnerID>(id);
	}

	inline void deserialize(ByteReader& _reader) override {
		username = _reader.readString();
	}
};
REGISTER_MESSAGE(HandshakeMessage)