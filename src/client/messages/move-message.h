#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are client side
struct MoveMessage : public MessageData {
    float dirX, dirY, dirZ; // Client to server
    float posX, posY, posZ; // Server to client
    OwnerID owner; // Server to client

    inline bool isValid() const override {
        return (dirX >= 0 && dirY >= 0 && dirZ >= 0)
            && (dirX <= 1 && dirY <= 1 && dirZ <= 1);
    }

    inline void serialize(ByteWriter& _writer) override {
        _writer.write<float>(dirX);
        _writer.write<float>(dirY);
        _writer.write<float>(dirZ);
    }

    inline void deserialize(ByteReader& _reader) override {
        posX = _reader.read<float>();
        posY = _reader.read<float>();
        posZ = _reader.read<float>();
        owner = _reader.read<OwnerID>();
    }
};

REGISTER_MESSAGE(MoveMessage)