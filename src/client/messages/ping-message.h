#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are client side
struct PingMessage : public MessageData {
	bool disconnected; // Client to server

	inline bool isValid() const override {
		return true;
	}

	inline void serialize(ByteWriter& _writer) override {
		_writer.write<bool>(disconnected);
	}

	inline void deserialize(ByteReader& _reader) override {
	}
};

REGISTER_MESSAGE(PingMessage)