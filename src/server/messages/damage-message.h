#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are server side
struct DamageMessage : public MessageData {
    uint ammount; // Server to client
    OwnerID owner; // Server to client
	
    inline bool isValid() const override {
        return true;
    }

    inline void serialize(ByteWriter& _writer) override {
        _writer.write<uint>(ammount);
        _writer.write<OwnerID>(id);
    }

    inline void deserialize(ByteReader& _reader) override {
    }
};
REGISTER_MESSAGE(DamageMessage)