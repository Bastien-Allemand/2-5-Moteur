#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are client side
struct HandshakeMessage : public MessageData {
	std::string username; // Client to server
	OwnerID id; // Server to client
	
	inline bool isValid() const override {
		return true;
	}

	inline void serialize(ByteWriter& _writer) override {
		_writer.writeString(username);
	}

	inline void deserialize(ByteReader& _reader) override {
		id = _reader.read<OwnerID>();
	}
};
REGISTER_MESSAGE(HandshakeMessage)