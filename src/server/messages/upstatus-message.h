#pragma once
// Local dependencies
#include "../common/utils/identification/hash.h"
#include "../common/framework/networking/message.h"
#include "../common/framework/networking/message-registry.h"

// We are server side
struct UpStatus_Message : public MessageData
{
	uint health;
	uint score;
	std::string username;
	OwnerID ownerid;
	inline bool isValid() const override {
		return true;
	}
	inline void serialize(ByteWriter& _writer) override {
		_writer.write<int>(health);
		_writer.write<int>(score);
		_writer.write<OwnerID>(ownerid);
		_writer.writeString(username);
	}
	inline void deserialize(ByteReader& _reader) override {
		//NO RECEIVE
	}
};
REGISTER_MESSAGE(UpStatus_Message)