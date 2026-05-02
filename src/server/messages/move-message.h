#pragma once

// External dependencies
#include <framework/networking/message.h>
#include <framework/networking/message-registry.h>

// We are server side
struct MoveMessage : public MessageData {
    float dirX, dirY, dirZ; // Client to server
    float posX, posY, posZ; // Server to client
    OwnerID owner; // Server to client

    inline bool isValid() const override {
        return (dirX >= 0 && dirY >= 0 && dirZ >= 0)
            && (dirX <= 1 && dirY <= 1 && dirZ <= 1);
    }

    inline void serialize(ByteWriter& _writer) override {
        _writer.write<float>(posX);
        _writer.write<float>(posY);
        _writer.write<float>(posZ);
        _writer.write<OwnerID>(owner);
    }

    inline void deserialize(ByteReader& _reader) override {
        dirX = _reader.read<float>();
        dirY = _reader.read<float>();
        dirZ = _reader.read<float>();
    }
};

REGISTER_MESSAGE(MoveMessage)