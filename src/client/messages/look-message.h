#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are client side
struct LookMessage : public MessageData {
    float posX, posY, posZ; // Client to server
    float rotX, rotY, rotZ, rotW; // Server to client
    OwnerID owner; // Server to client

    inline bool isValid() const override {
        return true;
    }

    inline void serialize(ByteWriter& _writer) override {
        _writer.write(posX);
        _writer.write(posY);
        _writer.write(posZ);
    }

    inline void deserialize(ByteReader& _reader) override {
        rotX = _reader.read<float>();
        rotY = _reader.read<float>();
        rotZ = _reader.read<float>();
        rotW = _reader.read<float>();
        owner = _reader.read<OwnerID>();
    }
};

REGISTER_MESSAGE(LookMessage)