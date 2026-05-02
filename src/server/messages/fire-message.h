#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are server side
struct FireMessage : public MessageData {
    float posX, posY, posZ; // Server to client
    float rotX, rotY, rotZ, rotW; // Server to client
    OwnerID owner; // Server to client
	
    inline bool isValid() const override {
        return true;
    }

    inline void serialize(ByteWriter& _writer) override {
        _writer.write<float>(posX);
        _writer.write<float>(posY);
        _writer.write<float>(posZ);
        
        _writer.write<float>(rotX);
        _writer.write<float>(rotY);
        _writer.write<float>(rotZ);
        _writer.write<float>(rotW);
        
        _writer.write<OwnerID>(owner);
    }

    inline void deserialize(ByteReader& _reader) override {
    }
};
REGISTER_MESSAGE(FireMessage)