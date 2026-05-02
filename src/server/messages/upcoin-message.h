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
        _writer.write<float>(PosX);
        _writer.write<float>(PosY);
        _writer.write<bool>(alive);
    }
    inline void deserialize(ByteReader& _reader) override {
        //NO RECEIVE
    }
};
REGISTER_MESSAGE(UpCoin_Message)