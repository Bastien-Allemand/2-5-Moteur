#pragma once
// Local dependencies
#include "../common/utils/identification/hash.h"
#include "../common/framework/networking/message.h"
#include "../common/framework/networking/message-registry.h"

// We are client side
struct UpStatus_Message : public MessageData {
	uint health;
	uint score;
	std::string username;
	OwnerID ownerid;
	
	inline bool isValid() const override {
		return true;
	}
	inline void serialize(ByteWriter& _writer) override {
		//NOTHING TO SEND
	}
	inline void deserialize(ByteReader& _reader) override {
		health = _reader.read<int>();
		score = _reader.read<int>();
		ownerid = _reader.read<OwnerID>();
		username = _reader.readString();
	}
};
REGISTER_MESSAGE(UpStatus_Message)