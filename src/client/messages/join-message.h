#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are client side
struct PlayerJoinedMessage : public MessageData {
	OwnerID owner; // Server to client

	inline bool isValid() const override {
		return true;
	}

	inline void serialize(ByteWriter& _writer) override {
	}

	inline void deserialize(ByteReader& _reader) override {
		owner = _reader.read<OwnerID>();
	}
};

REGISTER_MESSAGE(PlayerJoinedMessage)