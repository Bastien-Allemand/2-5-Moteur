#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are server side
struct PingMessage : public MessageData {
	bool disconnected; // Client to server

	inline bool isValid() const override {
		return true;
	}

	inline void serialize(ByteWriter& _writer) override {
	}

	inline void deserialize(ByteReader& _reader) override {
		_reader.read<bool>();
	}
};

REGISTER_MESSAGE(PingMessage)