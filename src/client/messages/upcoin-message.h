#pragma once
// Local dependencies
#include "../common/utils/identification/hash.h"
#include "../common/framework/networking/message.h"
#include "../common/framework/networking/message-registry.h"

struct UpCoin_Message : public MessageData
{
    float PosX, PosY;
    bool alive;

    inline bool isValid() const override {
        return true;
    }
    inline void serialize(ByteWriter& _writer) override {
        //NOTHING TO SEND
    }
    inline void deserialize(ByteReader& _reader) override {
        PosX = _reader.read<float>();
        PosY = _reader.read<float>();
        alive = _reader.read<bool>();
    }
};
REGISTER_MESSAGE(UpCoin_Message)