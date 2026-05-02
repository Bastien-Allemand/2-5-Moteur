#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are client side
struct FireMessage : public MessageData {
    float posX, posY, posZ; // Server to client
    float rotX, rotY, rotZ, rotW; // Server to client
    OwnerID owner; // Server to client
	
    inline bool isValid() const override {
        return true;
    }

    inline void serialize(ByteWriter& _writer) override {
    }

    inline void deserialize(ByteReader& _reader) override {
        posX = _reader.read<float>();
        posY = _reader.read<float>();
        posZ = _reader.read<float>();
        
        rotX = _reader.read<float>();
        rotY = _reader.read<float>();
        rotZ = _reader.read<float>();
        rotW = _reader.read<float>();
        
        owner = _reader.read<OwnerID>();
    }
};
REGISTER_MESSAGE(FireMessage)