#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are client side
struct DamageMessage : public MessageData {
    uint ammount; // Server to client
    OwnerID owner; // Server to client
	
    inline bool isValid() const override {
        return true;
    }

    inline void serialize(ByteWriter& _writer) override {
    }

    inline void deserialize(ByteReader& _reader) override {
        ammount = _reader.read<uint>();
        owner = _reader.read<OwnerID>();
    }
};
REGISTER_MESSAGE(DamageMessage)